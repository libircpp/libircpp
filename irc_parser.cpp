#include "irc_parser.hpp"

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1
#define BOOST_SPIRIT_USE_PHOENIX_V3          1

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <string>
#include <ostream>

struct prefix { optional_string nick, user, host; };

std::ostream& operator<<(std::ostream& os, const prefix& pfx) {
	if(pfx.nick) os << '<' << *pfx.nick << '>';

	if(pfx.nick && pfx.user) os << "!";

	if(pfx.user) os << '<' << *pfx.user << '>';

	if(pfx.host 
	&&( pfx.nick || pfx.user )) os << "@";

	if(pfx.host) os << '<' <<  *pfx.host << '>';
	return os;
}

void prnt_pfx(const prefix& pfx) {
	std::cout << pfx << '\n';
}

BOOST_FUSION_ADAPT_STRUCT(
	::prefix,
	(optional_string, nick)
	(optional_string, user)
	(optional_string, host)
)

namespace qi =boost::spirit::qi;
namespace phx=boost::phoenix;

template<typename T>
using rule=qi::rule<std::string::const_iterator, T()>;

void irc_parser::parse_message(const std::string& message) {
	auto first=message.cbegin(), last=message.cend();

	qi::lit_type    lit;
	qi::char_type   char_;
	qi::space_type  space;
	qi::_1_type     _1;
	qi::_2_type     _2;
	qi::_3_type     _3;

	qi::alpha_type  alpha;
	qi::alnum_type  alnum;

	qi::lexeme_type lexeme;
	qi::attr_type   attr;

 
	rule<std::string> nick = alpha >> *( alnum | char_("-[]\\`^{}") );

	rule<std::string> host = +~char_(' ');
 	//RFC 1459 is really vague about user, but no "?" seems like the best way to to deal with it
	rule<std::string> user = +~char_(" ?@");

	rule<std::string> word =+~char_(' ');
	rule<std::string> line =lit(':') >> *char_;

	rule<prefix> prefix_parser = 
		lit(':') >> ( nick                    >> -( '!' >> user )        >> -( '@' >> host ) >> lexeme[ ' ' ]
		            | attr(optional_string{}) >> attr(optional_string{}) >> host             >> lexeme[ ' ' ]
		            ) 
		            ;
		 
	bool success=qi::phrase_parse(
		first, last,

		 -prefix_parser[prnt_pfx] >> 			
			( ("PRIVMSG" >> +(!lit(':') >> word) >> line)  [ phx::bind(phx::ref(on_privmsg), _1, _2)     ]
			| ("NOTICE"  >> word >> line)                  [ phx::bind(phx::ref(on_notice),  _1, _2)     ]
			| ("MODE"    >> word >> line)                  [ phx::bind(phx::ref(on_mode),    _1, _2)     ]
			| ("TOPIC"   >> word >> line)                  [ phx::bind(phx::ref(on_topic),   _1, _2)     ]
			| ("KICK"    >> word >> word >> -line)         [ phx::bind(phx::ref(on_kick),    _1, _2, _3) ]
			| ("PING"    >> word >> -word)                 [ phx::bind(phx::ref(on_ping),    _1, _2)     ]
			| ("PONG"    >> word >> -word)                 [ phx::bind(phx::ref(on_pong),    _1, _2)     ]
			)
		, space
	);
	if(!success) {
		std::cerr << "ERROR PARSING: " <<  message << std::endl;
	}
}
