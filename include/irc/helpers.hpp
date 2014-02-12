
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_HELPERS_HPP
#define IRC_HELPERS_HPP

#include "users.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace irc {

template<typename TypeWithUsers, typename OutIter>
OutIter users_as_string(const TypeWithUsers& val, OutIter out) {
	return std::transform(val.begin_users(), val.end_users(), out,
		[](const user& u) { return u.get_nick(); }
	);
}

template<typename TypeWithUsers>
std::vector<std::string> users_as_string(const TypeWithUsers& val) {
	std::vector<std::string> out;
	users_as_string(val, std::back_inserter(out));
	return out;
}

} //namespace irc

#endif //IRC_HELPERS_HPP
