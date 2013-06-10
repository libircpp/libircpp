#ifndef SESSION_HPP
#define SESSION_HPP

#include "parser.hpp"
#include "types.hpp"

#include <memory> //shared_ptr
#include <string>
#include <vector>
#include <unordered_map>

namespace irc {

class session {
//member types 
	using channel_container                 =std::unordered_map<std::string, shared_channel>;
	using channel_iterator                  =channel_container::iterator;

	using user_container                    =std::unordered_map<std::string, shared_user>;
	using user_iterator                     =user_container::iterator;
//member variables
	parser parser_;
	std::shared_ptr<connection>           	 connection__;
	channel_container                        channels;
	user_container                           users;
	std::string                              nick, user_name, motd;
//callback
	sig_s                                    on_motd;
	sig_ch                                   on_join_channel;
	sig_s                                    on_notice;
	sig_usr_s                                on_user_notice;
//helper
	void prepare_connection();
	channel_iterator create_new_channel(const std::string& channel_name);
	channel_iterator get_or_create_channel(const std::string& channel_name);

	user_iterator create_new_user(const std::string& user_name, 
	                              const prefix& pfx);
	user_iterator get_or_create_user(const prefix& pfx);
	user_iterator get_or_create_user(const std::string& nick);
//handlers
	void handle_privmsg(const prefix&                   pfx,
	                    const std::vector<std::string>& target,
	                    const std::string&              content);

	void handle_notice (const prefix&                   pfx,
	                    const std::string&              nick,
	                    const std::string&              msg);
	
	void handle_ping(   const prefix&                   pfx,	
	                    const std::string&              channel,
	                    const optional_string&          msg);

	void handle_topic(  const std::string&              channel,
	                    std::string                     topic);

	void handle_join(   const prefix&                   pfx,	
	                    const std::string&              channel);

	void handle_part(   const prefix&                   pfx,	
	                    const std::string&              channel,
	                    const optional_string&          msg);

	void handle_quit(   const prefix&                   pfx,	
	                    const std::string&              channel);

	void handle_reply(  const prefix&                   pfx, 
	                    int                             rpl, 
	                    const std::vector<std::string>& params);
//deleted functions
	session(const session&)           =delete;
	session(session&&)                =delete;
	session& operator=(const session&)=delete;
	session& operator=(session&&)     =delete;
public:
	session(std::shared_ptr<connection> connection_,
	        std::string nick, std::string user_name_);


	const std::string& get_nick() const;

//async interface
	void async_join(const std::string& channel_name);
	void async_privmsg(const std::string& target, const std::string& msg);
	void async_change_nick(const std::string& target);

	void stop(); //async_stop?

	template<typename F> 
	bsig::connection connect_on_motd(F&& f);
	template<typename F> 
	bsig::connection connect_on_join_channel(F&& f);
	template<typename F> 
	bsig::connection connect_on_notice(F&& f);
}; //class session



template<typename F> 
bsig::connection session::connect_on_motd(F&& f) { 
	return on_motd.connect(std::move(f)); 
}
template<typename F> 
bsig::connection session::connect_on_join_channel(F&& f) {
	return on_join_channel.connect(std::move(f));
}
template<typename F> 
bsig::connection session::connect_on_notice(F&& f) {
	return on_notice.connect(std::move(f));
}

} //namespace irc

#endif //SESSION_HPP

