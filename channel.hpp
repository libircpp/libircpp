#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "connection.hpp"

#include <boost/signals.hpp>

#include <string>
#include <chrono>
#include <unordered_map>

namespace irc {

enum user_lvl {
	none, operator_, permission
};


std::tuple<std::string, user_lvl> nick_lvl(std::string nick);


class message {
public:
	std::chrono::system_clock::time_point time_stamp;
	std::string                           content;
	std::string                           user;
	message(std::chrono::system_clock::time_point time_stamp_, 
	        std::string                           content_, 
	        std::string                           user);
}; //class message

class channel {
	using user_container      =std::unordered_map<std::string, shared_prefix>;
	using message_container   =std::vector<message>;

	using user_iterator       =user_container::iterator;
	using message_iterator    =std::vector<message>::iterator;

	std::string                   name,
	                              topic;
	user_container                users;
	message_container             log;
//callbacks
	sig_2s                        on_message;
	sig_p_s                       on_user_join;
	sig_p_s_os                    on_user_leave;
//	sig                           on_user_quit;
//	sig                           on_topic;
//	sig                           on_kick;
//helpers
	user_iterator get_or_create_user(const prefix& pfx);
	user_iterator get_or_create_user(const std::string& str);
public:
	channel()=default;
	channel(const channel&)=delete;
	channel(channel&& other)=delete; //unfortunatly as signals are non movable

	channel(std::string name);

	message_iterator add_message(const prefix& pfx, std::string content);

	void add_user(const std::string& nick);
	void add_user(const prefix& pfx);

	void remove_user(const std::string& nick, const optional_string& msg);
	void set_topic(std::string str);
//getters 
	const std::string& get_name() const;
//registration
	template<typename F> boost::signals::connection connect_on_message(F&& f)
	{ return on_message.connect(std::move(f)); }

	template<typename F> boost::signals::connection connect_on_user_join(F&& f)
	{ return on_user_join.connect(std::move(f)); }

	template<typename F> boost::signals::connection connect_on_user_leave(F&& f)
	{ return on_user_leave.connect(std::move(f)); }
}; //class channel

} //namespace irc

#endif //CHANNEL_HPP
