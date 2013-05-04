#include "session.hpp"

#include <tuple> //tie
#include <sstream> //ostringstream
#include <stdexcept> //runtime_error

namespace irc {


message::message(std::chrono::system_clock::time_point time_stamp_, 
                 std::string                           content_, 
                 shared_prefix                         user_)
:	time_stamp { std::move(time_stamp_) }
,	content    { std::move(content)     }
,	user       { std::move(user)        }
{	}

channel::channel(std::string name_) 
:	name { std::move(name_) }
{	}

void channel::set_topic(std::string str) {
	topic=std::move(str);
	//TODO call on_topic
}


channel::user_iterator channel::get_or_create_user(const std::string& nick) {
	auto it=users.find(nick);	

	if(it==users.cend()) {
		bool success;

		std::tie(it, success)=users.emplace(
			nick,
			std::make_shared<prefix>(
				 nick, 
				 optional_string{},
				 optional_string{}
			)
		);

		if(!success) 
			throw std::runtime_error("Unable to insert new user: " + nick); 

		//TODO on new user (ermm well not quite, the update where necessary should be done first
	}

	return it;
}

channel::user_iterator channel::get_or_create_user(const prefix& pfx) {
	assert(pfx.nick);

	auto it=get_or_create_user(*pfx.nick);

	//TODO: optimise, and 'or' the optionals
	*it->second=pfx; //update the users credentials 

	//TODO on new user
	return it;
}

channel::message_iterator channel::add_message(const prefix& pfx, std::string content) {
	auto shared_user_it=get_or_create_user(pfx);
	assert(shared_user_it->second); //shared_ptr is valid
	
	log.emplace_back(
			std::chrono::system_clock::now(),
			std::move(content),
			shared_user_it->second
	);

	return log.end()-1;
}


session::channel_iterator session::create_new_channel(const std::string& name) {
	assert(channels.count(name)==0);

	channel_iterator it;
	bool             success;
	
	std::tie(it, success)=channels.emplace(name, name);

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
                             const std::string& target,
                             const std::string& content) {
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

	it->second.add_message(pfx, content);
}

void session::handle_topic(const std::string& channel, std::string topic) {
	auto it=get_or_create_channel(channel);
	it->second.set_topic(std::move(topic));
}



/*
** numeric responses from the server
*/
void session::handle_reply(int rp, const std::vector<std::string>& params) {
	switch(rp) {
	case 372: 
	{
		std::ostringstream oss; //TODO optimise for size=1 case?
		std::copy(params.cbegin(), params.cend(), 
			std::ostream_iterator<std::string>(oss, ""));
		motd+=oss.str();
		break;
	}
	case 375: //RPL_MOTDSTART
		motd.clear(); 
		break;
	case 376: //RPL_ENDOFMOTD
		on_motd(motd);
		break;
	case 353: //RPL_NAMREPLY
		if(params.size() > 3) { //
			auto it=get_or_create_channel(params[2]);
			std::for_each(params.cbegin() + 3, params.cend(),
				[&](const std::string& param) { it->second.add_user(param); });
		}
		break;
	default:
		//log unkown numeric response
		break;
	}
}

} //namespace irc
