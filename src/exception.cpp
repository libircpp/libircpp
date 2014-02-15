#include "exception.hpp"

#include <sstream>

namespace irc {

exception::exception(std::string what, std::string file, unsigned line,
	std::string function, std::chrono::system_clock::time_point occurred)
:	what_     { std::move(what)     }
,	file_     { std::move(file)     }
,	function_ { std::move(function) }
,	line_     { line                }
,	occurred_ { std::move(occurred) }
{	}

const char* exception::what() const noexcept { return what_.c_str(); }
const std::string& exception::file()     const noexcept { return file_; }
const std::string& exception::function() const noexcept { return function_; }
unsigned exception::line()               const noexcept { return line_; }

std::ostream& operator<<(std::ostream& os, const exception& ex) {
	return os << "Exception trown from function: " << ex.function()
	          << " in " << ex.file() << ':' << ex.line()
	          << " details: " << ex.what();
}

std::string to_string(const exception& ex) {
	std::ostringstream oss;
	oss << ex;
	return oss.str();
}

} //namespace irc
