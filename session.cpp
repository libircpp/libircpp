#include "session.hpp"

#include "numeric_replies.hpp"

#include "util.hpp"

#include <tuple> //tie
#include <sstream> //ostringstream
#include <stdexcept> //runtime_error

namespace irc {

void session::prepare_connection() {
	std::cout << "connected" << std::endl;
	assert(connection__);

	connection__->connect_on_privmsg(
		std::bind(&session::handle_privmsg, this, ph::_1, ph::_2, ph::_3));

	connection__->connect_on_reply(
		std::bind(&session::handle_reply,   this, ph::_1, ph::_2, ph::_3));
	
	connection__->connect_on_join(
		std::bind(&session::handle_join,    this, ph::_1, ph::_2));

	connection__->connect_on_part(
		std::bind(&session::handle_part,    this, ph::_1, ph::_2, ph::_3));
		
	connection__->connect_on_ping(
		std::bind(&session::handle_ping,    this, ph::_1, ph::_2, ph::_3));

	connection__->async_read();

	connection__->async_write("USER "+user+" 0 * :test user\r\n");
	connection__->async_write("NICK "+nick+"\r\n");
	connection__->async_write("JOIN #bown_fox\r\n");
}

session::session(std::shared_ptr<connection> connection_, 
                 std::string nick_, 
                 std::string user_) 
:	connection__ { std::move(connection_) }
,	nick         { std::move(nick_)       } 
,	user         { std::move(user_)       } 
{	
	prepare_connection();
}

session::channel_iterator session::create_new_channel(const std::string& name) {
	assert(channels.count(name)==0);

	channel_iterator it;
	bool             success;
	
	std::tie(it, success)=channels.emplace(name, util::make_unique<channel>(name));

	if(!success)
		throw std::runtime_error("Unable to insert new channel: " + name); 

	//TODO: on_new_channel;

	return it;
}

session::channel_iterator session::get_or_create_channel(const std::string& channel_name) {
	auto it=channels.find(channel_name);

	if(it!=channels.cend())
		return it;
	else 
		return create_new_channel(channel_name);
}

/*
** HANDLERS
*/ 
void session::handle_privmsg(const prefix& pfx,
                             const std::vector<std::string>& targets,
                             const std::string& content) {
	const std::string& target=targets[0];
	channel_iterator it;

	if(target == nick) { //1 to 1
		if(pfx.nick) { //nick is an optional
			it=get_or_create_channel(*pfx.nick);
		}
		else {
			//TODO log error
		}
	}
	else { //1 to channel 
		it=get_or_create_channel(target);
	}

	it->second->add_message(pfx, content);
}

void session::handle_topic(const std::string& channel, std::string topic) {
	auto it=get_or_create_channel(channel);
	it->second->set_topic(std::move(topic));
}

void session::handle_ping(const prefix& pfx, 
                          const std::string& server1,
                          const optional_string& server2) {
	std::cout << "Ping from: " << pfx << "  server1   " << server1 << "  server2? " << server2 << std::endl;
	std::ostringstream oss;
	oss << "PONG " << nick << " " << server1 << "\r\n";
	//ic->async_write("PONG" + server1 + "\r\n");
	connection__->async_write(oss.str());
}

void session::handle_join(const prefix& pfx,
                          const std::string& channel) {
	auto chan=get_or_create_channel(channel);
	if(pfx.nick && *pfx.nick != nick) {
		chan->second->add_user(pfx);
	}
	else {
		on_join_channel(*chan->second);
	}
}



void session::handle_part(const prefix& pfx,	
			              const std::string& channel,
                          const optional_string& msg) {
	//TODO have just get
	auto it=get_or_create_channel(channel);
	it->second->remove_user(*pfx.nick, msg);
}


/*
** numeric responses from the server
*/
void session::handle_reply(const prefix& pfx, int rp, 
                           const std::vector<std::string>& params) {
	numeric_replies nr=static_cast<numeric_replies>(rp);
	switch(nr) {
	case numeric_replies::RPL_MOTD: 
	{
		std::ostringstream oss; //TODO optimise for size=1 case?
		if(!motd.empty()) oss << '\n';
		if(params.size() > 1) {
			std::copy(params.cbegin()+1, params.cend(), 
				std::ostream_iterator<std::string>(oss, " "));
		}
		motd+=oss.str();
		break;
	}
	case numeric_replies::RPL_MOTDSTART:
		motd.clear(); 
		break;
	case numeric_replies::RPL_ENDOFMOTD:
		on_motd(motd);
		break;
	case numeric_replies::RPL_NAMREPLY:
		if(params.size() > 3) { //
			auto it=get_or_create_channel(params[2]);
			std::for_each(params.cbegin() + 3, params.cend(),
				[&](const std::string& param) { 
					std::istringstream iss { param };
					std::for_each(
						std::istream_iterator<std::string> { iss },
						std::istream_iterator<std::string> {     },
						[&](const std::string& nick)
						{ it->second->add_user(nick); }
					);
				}
			);
		}
		break;
	default:
		std::cerr << "reply" << rp << std::endl;
		break;
	}
}

} //namespace irc
