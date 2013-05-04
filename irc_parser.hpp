#ifndef IRC_PARSER_HPP
#define IRC_PARSER_HPP

#include <boost/optional.hpp>
#include <boost/signals.hpp>

#include <string>

using optional_string=boost::optional<std::string>;

//TODO: move to it's own file
struct prefix { 
	optional_string nick, user, host; 
	prefix()=default;
	prefix(prefix&&)=default;
	prefix(const prefix&)=default;
	prefix(optional_string nick_,
	       optional_string user_, 
	       optional_string host_);

	prefix& operator=(prefix&&)=default;
	prefix& operator=(const prefix&)=default;
};
std::ostream& operator<<(std::ostream& os, const prefix& pfx);

using sig_p_s        =boost::signal<void(prefix, std::string)>;
using sig_p_2s       =boost::signal<void(prefix, std::string, std::string)>;

using sig_s          =boost::signal<void(std::string)>;
using sig_2s         =boost::signal<void(std::string, std::string)>;
using sig_s_os       =boost::signal<void(std::string, optional_string)>;
using sig_2s_os      =boost::signal<void(std::string, std::string, optional_string)>;

//using sig_vs_s       =boost::signal<void(std::vector<std::string>, std::string)>;

using sig_p_i_vs     =boost::signal<void(prefix, int, std::vector<std::string>)>;
using sig_p_vs_s     =boost::signal<void(prefix, std::vector<std::string>, std::string)>;
using sig_p_s_os     =boost::signal<void(prefix, std::string, optional_string)>;

class irc_parser {
	sig_p_vs_s  on_privmsg;
	sig_2s      on_notice;
	sig_2s      on_mode;
	sig_2s      on_topic;
	sig_2s_os   on_kick;
	sig_p_s_os  on_ping;
	sig_s_os    on_pong;
	sig_p_s     on_join;
	sig_p_s_os  on_part;
	sig_p_s     on_quit;
	sig_p_s     on_nick;
	sig_p_i_vs  on_reply;
public:
	template<typename F> boost::signals::connection connect_on_privmsg(F&& f)
	{ return on_privmsg.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_notice(F&& f)
	{ return on_notice.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_mode(F&& f)
	{ return on_mode.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_topic(F&& f)
	{ return on_topic.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_kick(F&& f)
	{ return on_kick.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_ping(F&& f)
	{ return on_ping.connect(std::forward<F>(f)); }
	
	template<typename F> boost::signals::connection connect_on_pong(F&& f)
	{ return on_pong.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_join(F&& f)
	{ return on_join.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_part(F&& f)
	{ return on_part.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_quit(F&& f)
	{ return on_quit.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_nick(F&& f)
	{ return on_nick.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_reply(F&& f)
	{ return on_reply.connect(std::forward<F>(f)); }

	void parse_message(const std::string& message); 
}; //class irc_parser

#endif //IRC_PARSER_HPP

