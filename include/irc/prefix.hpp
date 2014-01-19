
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PREFIX_HPP
#define PREFIX_HPP

#include "types.hpp"

#include <ostream>

namespace irc {
/**
    @ref irc::message prefix.
    Represented by either a server's hostname, e.g. irc.freenode.net,
    or an user hostmask, with the format nickname!username\@hostname.
*/
struct prefix { 
	optional_string nick, user, host; 
/**
    Constructor.
    @param nick_ User nickname.
    @param user_ User name.
    @param host_ User hostname.
*/
	prefix(optional_string nick_,
	       optional_string user_, 
	       optional_string host_);
/**
    Constructor.
    @param nick_ User nickname.
*/
	prefix(optional_string nick_);

/** Default constructor. */
	prefix()                        =default;

/** Copy constructor. */
	prefix(prefix&&)                =default;

/** Copy constructor. */
	prefix(const prefix&)           =default;
/**
    Assignment operator.
    @return Prefix reference.
*/
	prefix& operator=(prefix&&)     =default;
/**
    Assignment operator.
    @return Prefix reference.
*/
	prefix& operator=(const prefix&)=default;
};

std::ostream& operator<<(std::ostream& os, const prefix& pfx);

std::string to_string(const prefix& pfx);

} //namespace irc

#endif //PREFIX_HPP
