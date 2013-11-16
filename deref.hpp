#ifndef DEREF_HPP
#define DEREF_HPP

#include "types.hpp"
#include "user.hpp"

#include <cassert>

namespace irc {

inline user& deref(const shared_user& up) { 
	assert(up && "invalid shared_ptr");
	return *up.get(); 
}
inline const user& cderef(const shared_user& up) { 
	assert(up && "invalid shared_ptr");
	return *up.get(); 
}

} //namespace irc

#endif //DEREF
