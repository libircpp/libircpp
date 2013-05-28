#ifndef IRC_TYPES_HPP
#define IRC_TYPES_HPP

#include <boost/optional.hpp>
#include <boost/signals.hpp>

#include <string>

namespace irc {
	struct prefix;
	class connection;
	class user;
	class channel;
	class message;
	class session;

	namespace bsig=boost::signals;

	using optional_string=boost::optional<std::string>;
	using sig_p_2s       =boost::signal<void(prefix, std::string, std::string)>;
	using sig_s          =boost::signal<void(std::string)>;
	using sig_2s         =boost::signal<void(std::string, std::string)>;
	using sig_s_os       =boost::signal<void(std::string, optional_string)>;
	using sig_2s_os      =boost::signal<void(std::string, std::string, optional_string)>;
	using sig_p_vs_s     =boost::signal<void(prefix, std::vector<std::string>, std::string)>;
	using sig_p_s_os     =boost::signal<void(prefix, std::string, optional_string)>;
	using sig_ch         =boost::signal<void(channel&)>;
	using sig_p_s        =boost::signal<void(const prefix&, const std::string&)>;
	using sig_p_i_vs     =boost::signal<void(prefix, int, std::vector<std::string>)>;
	using sig_v          =boost::signal<void(void)>;

	using sig_usr_s      =boost::signal<void(user&, const std::string&)>;
	using sig_ch         =boost::signal<void(channel&)>;
	using sig_ch_s       =boost::signal<void(channel&, const std::string&)>;
	using sig_ch_usr     =boost::signal<void(channel&, user&)>;
	using sig_ch_usr_s   =boost::signal<void(channel&, user&, const std::string&)>;
	using sig_ch_usr_os  =boost::signal<void(channel&, user&, const optional_string&)>;

	using shared_prefix  =std::shared_ptr<prefix>;
	using shared_channel =std::shared_ptr<channel>;
	using shared_user    =std::shared_ptr<user>;
} //namespace irc

#endif //IRC_TYPES_HPP
