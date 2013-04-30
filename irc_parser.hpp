#ifndef IRC_PARSER_HPP
#define IRC_PARSER_HPP

#include <boost/optional.hpp>
#include <boost/signals.hpp>

#include <string>

using optional_string=boost::optional<std::string>;
using sig_2s         =boost::signal<void(std::string, std::string)>;
using sig_s_os       =boost::signal<void(std::string, optional_string)>;

using sig_2s_os      =boost::signal<void(std::string, std::string, optional_string)>;

using sig_vs_s       =boost::signal<void(std::vector<std::string>, std::string)>;

class irc_parser {
	sig_vs_s  on_privmsg;
	sig_2s    on_notice;

	sig_2s    on_mode;
	sig_2s    on_topic;
	sig_2s_os on_kick;

	sig_s_os  on_ping;
	sig_s_os  on_pong;

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


	void parse_message(const std::string& message); 
}; //class irc_parser

#endif //IRC_PARSER_HPP




///
////for testing 
//#include <iostream>
//
//int main() {
//	irc_parser irc_p;
//
//
//	irc_p.connect_on_privmsg([](std::vector<std::string> s1, std::string s2) {
//		std::cout << "Private message to \n";
//		int i=0;
//		for(auto s : s1) { 
//			std::cout << i << ":  " << s << '\t';
//			++i;
//		}
//		std::cout << "MSG: " << s2 << '\n';
//	});
//
//	
//	irc_p.parse_message("PRIVMSG #hello #world :foo bar");
//
//	/*
//	irc_p.parse_message("NOTICE hello world");
//	irc_p.connect_on_notice([](std::string s1, std::string s2) {
//			std::cout << "1: " << s1 << "\n2: " << s2 << "\n";
//		}
//	);
//
//	irc_p.connect_on_ping([](std::string s1, optional_string o1) {
//		std::cout << "1: " << s1 << '\n';
//		if(o1)	std::cout << "2: " << *o1 << "\n";
//	});
//
//	irc_p.parse_message("NOTICE foo bar");
//	irc_p.parse_message("PING foo bar");
//	irc_p.parse_message("PING foo");
//	*/
//}
