#include "irc_parser.hpp"

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1
#define BOOST_SPIRIT_USE_PHOENIX_V3          1

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

namespace qi =boost::spirit::qi;
namespace phx=boost::phoenix;

template<typename T>
using rule=qi::rule<std::string::const_iterator, T()>;

void irc_parser::parse_message(const std::string& message) {


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
