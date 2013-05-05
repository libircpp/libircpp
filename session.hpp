#ifndef SESSION_HPP
#define SESSION_HPP

#include "irc_connection.hpp"

#include <chrono>
#include <memory> //shared_ptr
#include <string>
#include <vector>
#include <unordered_map>

namespace irc {

enum user_lvl {
	none, opterator, permission
};

class channel;

using sig_ch=boost::signal<void(channel&)>;
using sig_p_s=boost::signal<void(const prefix&, const std::string&)>;

using shared_prefix=std::shared_ptr<prefix>;

class message {
	std::chrono::system_clock::time_point time_stamp;
	std::string                           content;
	std::string                           user;
public:
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
//	sig                           on_message;
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
	template<typename F> boost::signals::connection connect_on_user_join(F&& f)
	{ return on_user_join.connect(std::move(f)); }

	template<typename F> boost::signals::connection connect_on_user_leave(F&& f)
	{ return on_user_leave.connect(std::move(f)); }

}; //class channel

class session {
	using channel_container                 =std::unordered_map<std::string, std::unique_ptr<channel>>;
	using channel_iterator                  =channel_container::iterator;
	std::shared_ptr<irc_connection>          connection;
	channel_container                        channels;
	std::string                              nick, user,
	                                         motd;
//callback
	sig_s                                    on_motd;
	sig_ch                                   on_join_channel;

//helper
	void prepare_connection();
//handlers
	void handle_privmsg(const prefix& pfx,
	                    const std::string& target,
	                    const std::string& content);
	
	void handle_topic(const std::string& channel,
	                  std::string        topic);

	void handle_join(const prefix& pfx,	
	                 const std::string& channel);

	void handle_part(const prefix& pfx,	
	                 const std::string& channel,
	                 const optional_string& msg);


	void handle_reply(const prefix& pfx, int rpl, 
	                  const std::vector<std::string>& params);

	channel_iterator create_new_channel(const std::string& channel_name);
	channel_iterator get_or_create_channel(const std::string& channel_name);
public:
	session(std::shared_ptr<irc_connection> connection_,
	        std::string nick, std::string user_);
	session(const session&)           =delete;
	session(session&&)                =delete;
	session& operator=(const session&)=delete;
	session& operator=(session&&)     =delete;


	template<typename F> boost::signals::connection connect_on_motd(F&& f) 
	{ return on_motd.connect(std::move(f)); }

	template<typename F> 
	boost::signals::connection connect_on_join_channel(F&& f) {
		return on_join_channel.connect(std::move(f));
	}
}; //class session


} //namespace irc

#endif //SESSION_HPP

