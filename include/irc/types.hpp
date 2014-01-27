
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_TYPES_HPP
#define IRC_TYPES_HPP

#include <boost/optional.hpp>
#include <boost/signals2.hpp>

#include <string>

/**
 * irc is the top level namespace of libircpp, this contains all of the compoents
 * required to use the library.
 */
namespace irc {
	template<typename ImplType>
	class crtp_channel;

	template<typename ImplType>
	class crtp_user;

	struct prefix;
	class connection;
	class user_impl;
	class channel_impl;
	struct message;
	class session;
	class mode_block;
	struct mode_diff;

	using channel          =crtp_channel<channel_impl>;
	using user             =crtp_user<user_impl>;

	namespace bsig         =boost::signals2;

	using optional_string  =boost::optional<std::string>;
	using optional_prefix  =boost::optional<prefix>;

	using mode_entry       =std::pair<char, optional_string>;
	using mode_list        =std::vector<mode_entry>;

	using sig_p_2s         =bsig::signal<void(prefix, std::string, std::string)>;
	using sig_s            =bsig::signal<void(std::string)>;
	using sig_2s           =bsig::signal<void(std::string, std::string)>;
	using sig_s_os         =bsig::signal<void(std::string, optional_string)>;
	using sig_2s_os        =bsig::signal<void(std::string, std::string, optional_string)>;
	using sig_p_vs_s       =bsig::signal<void(prefix, std::vector<std::string>, std::string)>;
	using sig_p_s_os       =bsig::signal<void(prefix, std::string, optional_string)>;
	using sig_ch           =bsig::signal<void(channel&)>;
	using sig_p_s          =bsig::signal<void(const prefix&, const std::string&)>;
	using sig_p_i_vs       =bsig::signal<void(prefix, int, std::vector<std::string>)>;
	using sig_v            =bsig::signal<void(void)>;

	using sig_usr          =bsig::signal<void(user&)>;
	using sig_usr_s        =bsig::signal<void(user&, const std::string&)>;
	using sig_ch           =bsig::signal<void(channel&)>;
	using sig_ch_s         =bsig::signal<void(channel&, const std::string&)>;
	using sig_ch_usr       =bsig::signal<void(channel&, user&)>;
	using sig_ch_usr_s     =bsig::signal<void(channel&, user&, const std::string&)>;
	using sig_ch_usr_os    =bsig::signal<void(channel&, user&, const optional_string&)>;

	using sig_p_md         =bsig::signal<void(const prefix&, const mode_diff&)>;
	using sig_ch_p_usr_md  =bsig::signal<void(irc::channel&, irc::user&, const prefix&, const mode_diff&)>;

	using shared_prefix    =std::shared_ptr<prefix>;
	using shared_channel   =std::shared_ptr<channel_impl>;
	using shared_connection=std::shared_ptr<connection>;
	using shared_user      =std::shared_ptr<user_impl>;
} //namespace irc

#endif //IRC_TYPES_HPP
