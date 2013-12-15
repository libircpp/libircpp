#include "../util.hpp"

/*
#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>

#include <string>
#include <limits>
#include <stdexcept>

//this is for my own sanity
namespace ph=std::placeholders;

//HELPERS
namespace util {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args ) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)... ));
}

template<typename Unsigned=unsigned>
Unsigned stou(const std::string& str, std::size_t *idx=nullptr, int base=10) {
	unsigned long result=std::stoul(str, idx, base);

	if (result > std::numeric_limits<Unsigned>::max())
		throw std::out_of_range("stou: ");

	return result;
}

} //namespace util

#endif //UTIL_HPP
*/
