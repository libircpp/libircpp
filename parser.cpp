
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parser.hpp"
#include "prefix.hpp"

#define BOOST_RESULT_OF_USE_DECLTYPE         1
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1
#define BOOST_SPIRIT_USE_PHOENIX_V3          1

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <string>
#include <ostream>

//Must be in global namespace
BOOST_FUSION_ADAPT_STRUCT(
	irc::prefix,
	(irc::optional_string, nick)
	(irc::optional_string, user)
	(irc::optional_string, host)
)


namespace irc {

namespace qi =boost::spirit::qi;
namespace phx=boost::phoenix;

template<typename T>
using rule=qi::rule<std::string::const_iterator, T()>;


void parser::parse_message(const std::string& message) {
	auto first=message.cbegin(), last=message.cend();

	qi::lit_type    lit;
	qi::int_type    int_;
	qi::char_type   char_;
	qi::space_type  space;
	qi::_1_type     _1;
	qi::_2_type     _2;
	qi::_3_type     _3;
	qi::_a_type     _a;
	
	qi::lexeme_type lexeme;
	qi::attr_type   attr;
 
	//rule<std::string> nick = alpha >> *( alnum | char_("|-[]\\`^{}_") );
	rule<std::string> nick = +~char_(" :@?!"); 

	rule<std::string> host = +~char_(' ');
 	//RFC 1459 is really vague about user, but no "?" seems like the best way to to deal with it
	rule<std::string> user = +~char_(" @");

	rule<std::string> word =+(!lit("\r") >> ~char_(" "));
	rule<std::string> line =lexeme[lit(':') >> *~char_('\r')];
	//*(!lit("\r") >> char_)];

	rule<prefix> prefix_parser = 
		lit(':') >> ( nick                    >> -( '!' >> user )        >> -( '@' >> host ) >> lexeme[ ' ' ]
		            | attr(optional_string{}) >> attr(optional_string{}) >> host             >> lexeme[ ' ' ]
		            ) 
		            ;

	qi::rule<std::string::const_iterator, qi::locals<prefix>, qi::space_type> irc_rule =
		 -prefix_parser[ _a = _1 ] >> 			
			( ("PRIVMSG" >> +(!lit(':') >> word) >> line)  [ phx::bind(phx::ref(on_privmsg), _a, _1, _2) ]
			| ("NOTICE"  >> word >> line)                  [ phx::bind(phx::ref(on_notice),  _a, _1, _2) ]
			| ("MODE"    >> word >> word)                  [ phx::bind(phx::ref(on_mode),    _1, _2)     ]
			| ("TOPIC"   >> word >> line)                  [ phx::bind(phx::ref(on_topic),   _1, _2)     ]
			| ("KICK"    >> word >> word >> -line)         [ phx::bind(phx::ref(on_kick),    _1, _2, _3) ]
			| ("PING"    >> word >> -word)                 [ phx::bind(phx::ref(on_ping),    _a, _1, _2) ]
			| ("PONG"    >> word >> -word)                 [ phx::bind(phx::ref(on_pong),    _1, _2)     ]
			//TODO: joining can be a lot more complex than this, unsure if complex forms can come from server
			| ("JOIN"    >> word)                          [ phx::bind(phx::ref(on_join),    _a, _1)     ]
			| ("PART"    >> word >> -line)                 [ phx::bind(phx::ref(on_part),    _a, _1, _2) ]
			| ("QUIT"    >> line)                          [ phx::bind(phx::ref(on_quit),    _a, _1)     ]
			| ("NICK"    >> line)                          [ phx::bind(phx::ref(on_nick),    _a, _1)     ]
			//NUMERIC response	
			//| ( int_     >> (*(!lit(':') >> word) >> -line ))            [ phx::bind(phx::ref(on_reply),   _a, _1, _2) ]
			| ( int_     >> *(line | word))                [ phx::bind(phx::ref(on_reply),   _a, _1, _2) ]
			)
			;
#ifdef IRC_PRNT_DEBUG
	std::cout << message << std::endl;
#endif //IRC_PRNT_DEBUG
		 
	bool success=qi::phrase_parse(first, last, irc_rule, space);
	if(!success) {
		//handle this better
		std::cerr << "ERROR PARSING: " <<  message << std::endl;
	}
}

} //namespace irc
