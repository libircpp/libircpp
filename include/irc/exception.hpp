#ifndef IRC_EXCEPTION_HPP
#define IRC_EXCEPTION_HPP

#include <exception>
#include <ostream>
#include <string>
#include <chrono>

namespace irc {

class exception : public std::exception {
	std::string what_, file_, function_;
	unsigned line_;
	std::chrono::system_clock::time_point occurred_;
public:
	exception(std::string what, std::string file, unsigned line,
		std::string function, std::chrono::system_clock::time_point occurred=
			std::chrono::system_clock::now());

	virtual const char* what()    const noexcept override;
	const std::string& file()     const noexcept;
	const std::string& function() const noexcept;
	unsigned line()               const noexcept;
}; //class exception

std::ostream& operator<<(std::ostream& os, const exception& ex);
std::string to_string(const exception& ex);

#define IRC_MAKE_EXCEPTION(msg) \
	::irc::exception { msg, __FILE__, __LINE__, __func__ };

} //namespace irc

#endif //IRC_EXCEPTION_HPP
