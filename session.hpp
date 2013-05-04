#ifndef SESSION_HPP
#define SESSION_HPP

#include "irc_connection.hpp"

#include <chrono>
#include <memory> //shared_ptr
#include <string>
#include <vector>
#include <unordered_map>

namespace irc {

using shared_prefix=std::shared_ptr<prefix>;

class message {
	std::chrono::system_clock::time_point time_stamp;
	std::string                           content;
	shared_prefix                         user;
public:
	message(std::chrono::system_clock::time_point time_stamp_, 
	        std::string                           content_, 
	        shared_prefix                         user);
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
//	sig                           on_message;
//	sig                           on_user_join;
//	sig                           on_user_leave;
//	sig                           on_user_quit;
//	sig                           on_topic;
//	sig                           on_kick;

//helpers
	user_iterator get_or_create_user(const prefix& pfx);
	user_iterator get_or_create_user(const std::string& str);
public:
	channel()=default;
	channel(std::string name);

	message_iterator add_message(const prefix& pfx, std::string content);

	void add_user(std::string);

	void remove_user(shared_prefix);
	void remove_user(std::string nick);
	void set_topic(std::string str);
}; //class channel

class session {
	using channel_container                 =std::unordered_map<std::string, channel>;
	using channel_iterator                  =channel_container::iterator;
	std::shared_ptr<irc_connection>          connection;
	std::unordered_map<std::string, channel> channels;
	std::string                              nick,
	                                         motd;
//callback
	sig_s                                    on_motd;

//handlers
	void handle_privmsg(const prefix& pfx,
	                    const std::string& target,
	                    const std::string& content);
	
	void handle_topic(const std::string& channel,
	                  std::string        topic);
	
	void handle_reply(int rpl, const std::vector<std::string>& params);

	channel_iterator create_new_channel(const std::string& channel_name);
	channel_iterator get_or_create_channel(const std::string& channel_name);
public:


}; //class session


} //namespace irc

#endif //SESSION_HPP

