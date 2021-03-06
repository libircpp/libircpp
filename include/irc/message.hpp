
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

#include <vector>
#include <string>

namespace irc {
/**
    IRC message struct.
    Servers and clients send each other messages,
    which may or may not generate a reply.
*/
struct message {
/**
    Message prefix (optional).
*/
	optional_prefix          prefix;
/**
    Message command.
*/
	irc::command             command;
/**
    Message command parameters.
    RFC set this to a maximum of 15.
*/
	std::vector<std::string> params; 
}; //struct message

std::tuple<bool, message> parse_message(const std::string &raw_msg);

} //namespace irc

#endif //IRC_CLIENT_MESSAGE_HPP
