#define BOOST_SPIRIT_USE_PHOENIX_V3 1
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <boost/optional.hpp>
#include <boost/signals.hpp>

#include <string>

using optional_string=boost::optional<std::string>;
using sig_2s         =boost::signal<void(std::string, std::string)>;
using sig_s_os       =boost::signal<void(std::string, optional_string)>;

using sig_vs_s       =boost::signal<void(std::vector<std::string>, std::string)>;

namespace qi =boost::spirit::qi;
namespace phx=boost::phoenix;

class irc_parser {
	sig_vs_s on_privmsg;
	sig_2s   on_notice;
	sig_s_os on_ping;
	sig_s_os on_pong;

	template<typename T>
	using rule=qi::rule<std::string::const_iterator, T()>;
public:
	template<typename F> boost::signals::connection connect_on_privmsg(F&& f)
	{ return on_privmsg.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_notice(F&& f)
	{ return on_notice.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_ping(F&& f)
	{ return on_ping.connect(std::forward<F>(f)); }
	
	template<typename F> boost::signals::connection connect_on_pong(F&& f)
	{ return on_pong.connect(std::forward<F>(f)); }


	void parse_message(const std::string& message) {
		auto first=message.cbegin(), last=message.cend();

		qi::lit_type    lit;
		qi::char_type   char_;
		qi::space_type  space;
		qi::string_type string;

		qi::_1_type    _1;
		qi::_2_type    _2;

		rule<std::string> word =+(~char_(' '));
		rule<std::string> line =lit(':') >> *char_;

		rule<std::string> actor=char_("@#") >> word;


		bool success=qi::phrase_parse(
			first, last,
			( ("PRIVMSG" >> +(!lit(':') >> actor) >> line)  
			                               [ phx::bind(phx::ref(on_privmsg), _1, _2) ]
			| ("NOTICE"  >> word >> word)  [ phx::bind(phx::ref(on_notice),  _1, _2) ]
			| ("PING"    >> word >> -word) [ phx::bind(phx::ref(on_ping),    _1, _2) ]
			| ("PONG"    >> word >> -word) [ phx::bind(phx::ref(on_pong),    _1, _2) ]
			)
			, space
		);
		if(!success) {
			std::cerr << "ERROR PARSING: " <<  message << std::endl;
		}
	}
}; //class irc_parser


//for testing 
#include <iostream>

int main() {
	irc_parser irc_p;


	irc_p.connect_on_privmsg([](std::vector<std::string> s1, std::string s2) {
		std::cout << "Private message to \n";
		int i=0;
		for(auto s : s1) { 
			std::cout << i << ":  " << s << '\t';
			++i;
		}
		std::cout << "MSG: " << s2 << '\n';
	});

	
	irc_p.parse_message("PRIVMSG #hello #world :foo bar");

	/*
	irc_p.parse_message("NOTICE hello world");
	irc_p.connect_on_notice([](std::string s1, std::string s2) {
			std::cout << "1: " << s1 << "\n2: " << s2 << "\n";
		}
	);

	irc_p.connect_on_ping([](std::string s1, optional_string o1) {
		std::cout << "1: " << s1 << '\n';
		if(o1)	std::cout << "2: " << *o1 << "\n";
	});

	irc_p.parse_message("NOTICE foo bar");
	irc_p.parse_message("PING foo bar");
	irc_p.parse_message("PING foo");
	*/
}
