#include "channel.hpp"

#include <string>
#include <tuple>
#include <stdexcept>

namespace irc {

message::message(std::chrono::system_clock::time_point time_stamp_, 
                 std::string                           content_, 
                 std::string                           user_)
:	time_stamp { std::move(time_stamp_) }
,	content    { std::move(content_)    }
,	user       { std::move(user_)       }
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
			std::make_shared<prefix>(nick, optional_string{}, optional_string{})
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
		*pfx.nick
	);

	on_message(*pfx.nick, log.back().content);

	return log.end()-1;
}


void channel::add_user(const std::string& nick) {
	auto it=get_or_create_user(nick);
	on_user_join(*it->second, nick);
}
void channel::add_user(const prefix& nick) {
	auto it=get_or_create_user(nick);
	on_user_join(*it->second, *nick.nick);
}

void channel::remove_user(const std::string& nick, const optional_string& msg) {
	auto it=users.find(nick);
	if(it!=users.cend()) {
		auto pfxp=it->second;
		users.erase(it);
		on_user_leave(*pfxp, nick, msg);
	}
}

const std::string& channel::get_name() const {
	return name;
}

} //namespace irc
