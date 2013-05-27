#include "connection.hpp"
#include "session.hpp"
#include "channel.hpp"
#include "prefix.hpp"
#include "user.hpp"

#include "numeric_replies.hpp"

#include "util.hpp"

#include <tuple> //tie
#include <sstream> //ostringstream
#include <stdexcept> //runtime_error
#include <iostream>

namespace irc {

void session::prepare_connection() {
	assert(connection__);

	connection__->connect_on_privmsg(
		std::bind(&session::handle_privmsg, this, ph::_1, ph::_2, ph::_3));

	connection__->connect_on_notice(
		std::bind(&session::handle_notice,  this, ph::_1, ph::_2, ph::_3));

	connection__->connect_on_reply(
		std::bind(&session::handle_reply,   this, ph::_1, ph::_2, ph::_3));
	
	connection__->connect_on_topic(
		std::bind(&session::handle_topic,   this, ph::_1, ph::_2));

	connection__->connect_on_join(
		std::bind(&session::handle_join,    this, ph::_1, ph::_2));

	connection__->connect_on_part(
		std::bind(&session::handle_part,    this, ph::_1, ph::_2, ph::_3));

	connection__->connect_on_quit(
		std::bind(&session::handle_quit,    this, ph::_1, ph::_2));

	connection__->connect_on_ping(
		std::bind(&session::handle_ping,    this, ph::_1, ph::_2, ph::_3));

	

	connection__->async_read();

	connection__->async_write("USER "+user_name+" 0 * :test user\r\n");
	connection__->async_write("NICK "+nick+"\r\n");
}

session::session(std::shared_ptr<connection> connection_, 
                 std::string nick_, 
                 std::string user_name_) 
:	connection__ { std::move(connection_) }
,	nick         { std::move(nick_)       } 
,	user_name    { std::move(user_name_)  } 
{	
	prepare_connection();
}

session::channel_iterator session::create_new_channel(const std::string& name) {
	assert(channels.count(name)==0);

	channel_iterator it;
	bool             success;

	std::tie(it, success)=channels.emplace(
		name, std::make_shared<channel>(*this, name));

	if(!success)
		throw std::runtime_error("Unable to insert new channel: " + name); 

	return it;
}

session::channel_iterator session::get_or_create_channel(const std::string& channel_name) {
	auto it=channels.find(channel_name);

	if(it!=channels.cend())
		return it;
	else 
		return create_new_channel(channel_name);
}

session::user_iterator session::create_new_user(const std::string& name, 
                                                const prefix& pfx) {
	assert(users.count(name)==0);
	user_iterator it;
	bool          success;

	std::tie(it, success)=users.emplace(
		name, std::make_shared<user>(name, pfx));

	if(!success)
		throw std::runtime_error("Unable to insert new user: " + name); 

	return it;
}

session::user_iterator session::get_or_create_user(const std::string& user_name) {
	auto it=users.find(user_name);

	if(it!=users.cend())
		return it;
	else 
		return create_new_user(user_name, { user_name });
}

session::user_iterator session::get_or_create_user(const prefix& pfx) {
	assert(pfx.nick);
	auto it=users.find(*pfx.nick);

	if(it!=users.cend())
		return it;
	else 
		return create_new_user(*pfx.nick, pfx);
}


/*
** HANDLERS
*/ 
void session::handle_privmsg(const prefix& pfx,
                             const std::vector<std::string>& targets,
                             const std::string& content) {
	const std::string& target=targets[0];
	if(pfx.nick) { //nick is an optional
		auto user=get_or_create_user(pfx)->second; 
		assert(user);
		if(target == nick) { //1 to 1
			user->direct_message(content);
		}
		else { //1 to channel 
			auto chan=get_or_create_channel(target)->second;
			assert(chan);

			chan->message(user, content);
			user->channel_message(chan, content);
		}
	}
}


void session::handle_notice (const prefix&      pfx,
                             const std::string& target,
                             const std::string& msg) {
	if(pfx.nick) {
		auto user=get_or_create_user(pfx)->second;
		assert(user);
		user->notice(msg);
		on_user_notice(*user, msg);
	}
	else {
		on_notice(msg);
	}
}

void session::handle_topic(const std::string& channel, std::string topic) {
	auto chan=get_or_create_channel(channel)->second;
	assert(chan);
	chan->set_topic(std::move(topic));
}

void session::handle_ping(const prefix& pfx, 
                          const std::string& server1,
                          const optional_string& server2) {
	std::ostringstream oss;
	oss << "PONG " << nick << " " << server1 << "\r\n";
	connection__->async_write(oss.str());
}

void session::handle_join(const prefix& pfx,
                          const std::string& channel) {
	if(pfx.nick) {
		auto chan=get_or_create_channel(channel)->second;
		auto user=get_or_create_user(pfx)->second;
		assert(chan);
		assert(user);
		chan->user_join(user);
		if(user->get_nick() == nick) { //is_me?
			on_join_channel(*chan);
		}
	}
}

void session::handle_part(const prefix& pfx,	
                          const std::string& channel,
                          const optional_string& msg) {
	//TODO have just get
	auto chan=get_or_create_channel(channel)->second;
	auto user_it=get_or_create_user(pfx);
	auto user=user_it->second;

	assert(chan);
	assert(user);

	chan->user_part(user, msg);

	users.erase(user_it);
}

void session::handle_quit(const prefix& pfx,
                          const std::string& msg) {
	if(pfx.nick) {
		auto user_it=get_or_create_user(pfx); //todo: optimise
		auto user   =user_it->second;

		for(auto& channel : channels) {
			channel.second->user_quit(user, msg);
		}
		users.erase(user_it);
	}
	else {
		std::cerr << "QUIT: unknown nick prefix is: " << pfx << std::endl;
	}
}

// numeric responses from the server
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

			auto chan=get_or_create_channel(params[2])->second;
			assert(chan);

			std::for_each(params.cbegin() + 3, params.cend(),
				[&](const std::string& param) { 
					std::istringstream iss { param };
					std::for_each(
						std::istream_iterator<std::string> { iss },
						std::istream_iterator<std::string> {     },
						[&](const std::string& nick)
						{ 
							auto user=get_or_create_user(nick)->second;
							assert(user);
							chan->user_join(user); 
						}
					);
				}
			);
		}
		break;
	case numeric_replies::RPL_TOPIC:
		if(params.size() > 2) {
			auto chan=get_or_create_channel(params[1])->second;
			chan->set_topic(params[2]);
		}
		break;
	default:
		//std::cerr << "reply" << rp << std::endl;
		break;
	}
}

/*
** async interface
*/
void session::async_join(const std::string& channel_name) {
	std::ostringstream oss;
	oss << "JOIN " << channel_name << "\r\n";
	connection__->async_write(oss.str());
}
void session::async_privmsg(const std::string& target, const std::string& msg) {
	std::ostringstream oss;
	oss << "PRIVMSG " << target << " :" << msg << "\r\n";
	connection__->async_write(oss.str());
}
void session::async_change_nick(const std::string& desired_nick) {
	std::ostringstream oss;
	//oss << ':' << nick << " NICK " << desired_nick ;//<< "\r\n";
	oss << "NICK " << desired_nick << "\r\n";
	connection__->async_write(oss.str());
}


} //namespace irc
