
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//Created:     2014/01/17

#include "message.hpp"
#include "message_parser.hpp"

#include <string>
#include <tuple>

//This has it's own file because parser.hpp takes ages to compile

namespace irc {

std::tuple<bool, message> parse_message(const std::string &raw_msg) {
    using iterator_type =std::string::const_iterator;
    using message_parser=message_parser<iterator_type>;
 
    message_parser parser;
    message        msg;
    iterator_type  first=raw_msg.begin(),
	               last=raw_msg.end();

	qi::space_type space;

    bool r=qi::phrase_parse(first, last, parser, space, msg);
 
#ifdef IRC_DEBUG
    std::cout << raw_msg << '\n';
    if( r && iter == end ) {
        std::cout << "#prefix :" << msg.prefix  << '\n';
        std::cout << "#command:" << msg.command << '\n';
        std::cout << "#params :" << msg.params  << '\n';
    }
    else {
        std::cout << "Parsing failed\n";
    }
#endif

    return std::make_tuple(r, msg);
}

} //namespace irc
