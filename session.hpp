#ifndef SESSION_HPP
#define SESSION_HPP

#include "connection.hpp"
#include "channel.hpp"

#include <memory> //shared_ptr
#include <string>
#include <vector>
#include <unordered_map>

namespace irc {

class session {
	using channel_container                 =std::unordered_map<std::string, std::unique_ptr<channel>>;
	using channel_iterator                  =channel_container::iterator;
	std::shared_ptr<connection>           	 connection__;
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
	                    const std::vector<std::string>& target,
	                    const std::string& content);
	
	void handle_ping(const prefix& pfx,	
	                 const std::string& channel,
	                 const optional_string& msg);

	void handle_topic(const std::string& channel,
	                  std::string        topic);

	void handle_join(const prefix& pfx,	
	                 const std::string& channel);

	void handle_part(const prefix& pfx,	
	                 const std::string& channel,
	                 const optional_string& msg);

	void handle_quit(const prefix& pfx,	
	                 const std::string& channel);

	void handle_reply(const prefix& pfx, int rpl, 
	                  const std::vector<std::string>& params);

	channel_iterator create_new_channel(const std::string& channel_name);
	channel_iterator get_or_create_channel(const std::string& channel_name);
public:
	session(std::shared_ptr<connection> connection_,
	        std::string nick, std::string user_);
	session(const session&)           =delete;
	session(session&&)                =delete;
	session& operator=(const session&)=delete;
	session& operator=(session&&)     =delete;


//async interface
	void async_join(const std::string& channel_name);
	void async_privmsg(const std::string& target, const std::string& msg);


	template<typename F> boost::signals::connection connect_on_motd(F&& f) 
	{ return on_motd.connect(std::move(f)); }

	template<typename F> 
	boost::signals::connection connect_on_join_channel(F&& f) {
		return on_join_channel.connect(std::move(f));
	}
}; //class session

} //namespace irc

#endif //SESSION_HPP

