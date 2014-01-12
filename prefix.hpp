
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PREFIX_HPP
#define PREFIX_HPP

#include "types.hpp"

namespace irc {

struct prefix { 
	optional_string nick, user, host; 

	prefix(optional_string nick_,
	       optional_string user_, 
	       optional_string host_);

	prefix(optional_string nick_);

	prefix()                        =default;
	prefix(prefix&&)                =default;
	prefix(const prefix&)           =default;
	prefix& operator=(prefix&&)     =default;
	prefix& operator=(const prefix&)=default;

};

std::ostream& operator<<(std::ostream& os, const prefix& pfx);

} //namespace irc

#endif //PREFIX_HPP
