#include "version.hpp"

#include <boost/version.hpp>

#include <sstream>

namespace irc {

std::string compiler_version() {
	std::ostringstream oss;


	#if __clang__
		oss << "clang version " << __clang_major__ << '.' 
		    << __clang_minor__ << '.' << __clang_patchlevel__;
	#elif __GNUC__
		oss << "gcc version " <<  __GNUC__ << '.' 
		    << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__;
	#else
		oss << "unkown compiler"
	#endif

	//TODO intel and MSVC (when they support enough C++11)
	return oss.str();
}

std::string boost_version() {
	return "boost version " BOOST_LIB_VERSION;
}

std::string library_version() {
	std::ostringstream oss;
	oss << "libircpp version " << major_version << "." << minor_version;
	return oss.str();
}

std::string version_info() {
	std::ostringstream oss;
	oss << library_version()
	    << " compiled with: " << compiler_version()
	    << " using: " << boost_version();
	return oss.str();
}

} //namespace irc
