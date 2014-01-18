
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//Created:     2014/01/17

#ifndef IRC_CLIENT_MESSAGE_PARSER_HPP
#define IRC_CLIENT_MESSAGE_PARSER_HPP

//SPEEDS UP SPIRIT COMPILE TIMES
#define BOOST_RESULT_OF_USE_DECLTYPE         1
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1
#define BOOST_SPIRIT_USE_PHOENIX_V3          1
 
#include "types.hpp"
#include "prefix.hpp"
#include "message.hpp"
#include "command.hpp"
 
#include <boost/spirit/home/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
 
#include <string>
 
using params_type = decltype(irc::message::params);
 
BOOST_FUSION_ADAPT_STRUCT(
	irc::message,
	(irc::optional_prefix, prefix)
	(irc::command,        command)
	(params_type,          params)
)
BOOST_FUSION_ADAPT_STRUCT(
	irc::prefix,
	(irc::optional_string, nick)
	(irc::optional_string, user)
	(irc::optional_string, host)
)
 
namespace irc {
 
namespace qi =boost::spirit::qi;
namespace phx=boost::phoenix;
 
template <typename Iterator>
struct message_parser : qi::grammar<Iterator, message(), qi::space_type> {

    template<typename Val>
    using rule=qi::rule<Iterator, Val(), qi::space_type>;

    //space sensitive
    template<typename Val>
    using rule_ss=qi::rule<Iterator, Val()>;

	message_parser() : message_parser::base_type(msg) {
		qi::char_type   char_;
		qi::int_type    int_;
		qi::lit_type    lit;
		qi::attr_type   attr;
		qi::_1_type     _1;
		qi::_val_type   _val;
	/*
    The presence of a prefix is indicated with a single leading ASCII
    colon character (':', 0x3b), which MUST be the first character of the
    message itself.
    There MUST be NO gap (whitespace) between the colon and the prefix.
    The prefix is used by servers to indicate the true origin of the message.
    If the prefix is missing from the message, it is assumed to have originated
    from the connection from which it was received from.
    Clients SHOULD NOT use a prefix when sending a message;
    if they use one, the only valid prefix
    is the registered nickname associated with the client.
*/

/*
    The command MUST either be a valid IRC command
    or a three (3) digit number represented in ASCII text.
*/
		verbose_command.add
			("NICK",   command::nick)   ("KICK",    command::kick)
			("ERROR",  command::error)  ("MODE",    command::mode)
			("QUIT",   command::quit)   ("TOPIC",   command::topic)
			("PING",   command::ping)   ("PONG",    command::pong)
			("NOTICE", command::notice) ("JOIN",    command::join)
			("PART",   command::part)   ("PRIVMSG", command::privmsg);
		
		pfx %= lit(':') >> 
			 ( +~char_(" :@?!")        >> -( '!' >> +~char_(" @") )  >> -( '@' >> +~char_(' ') )
			 | attr(optional_string{}) >> attr(optional_string{})    >>  +~char_(' ') 
			 )
			 ;
	
		cmd     %= verbose_command | int_[ _val=phx::bind(to_command, _1) ]; //sorry

		to_end  %= lit(':') >> *~char_("\r");
		word    %= +~char_(" \r");

		params  %= +( to_end | word );

		msg     %= -pfx >> cmd >> params;
    }
private:
    qi::symbols<char, command> verbose_command;
    rule<command>              cmd;  //command clashes with irc::command
    rule_ss<prefix>            pfx; //space sensitive
    rule_ss<std::string>       to_end;
    rule_ss<std::string>       word;
    rule<params_type>          params;
    rule<message>              msg;
};

}
 
#endif //IRC_CLIENT_IMPL_PARSE_IPP
