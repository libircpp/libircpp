
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//Created:     2014/01/17

#ifndef IRC_CLIENT_MESSAGE_HPP
#define IRC_CLIENT_MESSAGE_HPP

#include "types.hpp"
#include "prefix.hpp"
#include "command.hpp"

namespace irc {

struct message {
	//messages maynot have prefixs
	optional_prefix          prefix;
	irc::command             command;
	std::vector<std::string> params; 
}; //struct message

std::tuple<bool, message> parse_message(const std::string &raw_msg);

} //namespace irc

#endif //IRC_CLIENT_MESSAGE_HPP
