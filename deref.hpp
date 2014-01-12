
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DEREF_HPP
#define DEREF_HPP

#include "types.hpp"
//#include "user.hpp"

#include <cassert>

namespace irc {


struct deref {
	template<typename Ptr>
	typename Ptr::element_type& operator()(const Ptr& ptr) const {
		return *ptr.get();
	}
};


struct second_deref {
	template<typename K, typename Ptr>
	typename Ptr::element_type& operator()(const std::pair<K, Ptr>& pair) const {
		return *pair.second;
	}
};

/*

inline user& deref(const shared_user& up) { 
	assert(up && "invalid shared_ptr");
	return *up.get(); 
}
inline const user& cderef(const shared_user& up) { 
	assert(up && "invalid shared_ptr");
	return *up.get(); 
}

inline const channel& cderef_chan(const shared_channel& up) { 
	assert(up && "invalid shared_ptr");
	return up->second.get(); 
}
*/

} //namespace irc

#endif //DEREF
