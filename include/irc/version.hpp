#ifndef IRC_VERSION_HPP
#define IRC_VERSION_HPP

#include <string>

namespace irc {

#define IRCPP_MAJOR_VERSION 0
#define IRCPP_MINOR_VERSION 0

constexpr unsigned major_version=IRCPP_MAJOR_VERSION;
constexpr unsigned minor_version=IRCPP_MINOR_VERSION;

std::string compiler_version();
std::string boost_version();
std::string library_version();
std::string version_info();

} //namespace irc

#endif //VERSION_HPP
