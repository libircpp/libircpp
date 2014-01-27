
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_USER_HPP
#define IRC_USER_HPP

#include "crtp_user.hpp"
#include "types.hpp"
#include "modes.hpp"
#include "prefix.hpp"

#include <string>

namespace irc {

/**
 * The user class models and IRC user
 */
class user_impl : public crtp_user<user_impl> {
	std::string  nick_;
	prefix       pfx_;
	mode_block   modes_;
//signals
	sig_ch_usr_s on_channel_message;
	sig_usr_s    on_direct_message;
	sig_usr_s    on_nick_change;
	sig_usr_s    on_notice;

//deleted functions
	user_impl(const user_impl&)           =delete;
	user_impl(user_impl&&)                =delete;
	user_impl& operator=(const user_impl&)=delete;
	user_impl& operator=(user_impl&&)     =delete;
public:
	/**
	 * Constructor.
	 * @param nick_ The user nickname.
	 */
	user_impl(std::string nick_);
	/**
	 * Constructor.
	 * @param nick_ The user nickname.
	 * @param pfx_  The user hostmask.
	 */
	user_impl(std::string nick_, prefix pfx_);

	//USER INTERFACE
	/**
	 * @brief returns the nick for this user
	 * @return the name
	 */
	const std::string& get_nick_impl()    const;
	/**
	 * @brief returns the prefix for this user
	 * @return the prefix
	 */
	const prefix& get_prefix_impl() const;
	/**
	 * @brief returns the mode block for this user
	 * @return the mode block
	 */
	mode_block& get_modes_impl();
	/**
	 * @brief const version
	 * @return the mode block
	 * @see get_modes
	 */
	const mode_block& get_modes_impl() const;

	//SYSTEM INTERFACE 
	void set_nick(std::string nick_);
	void set_prefix(prefix pfx_);

	void channel_message(const shared_channel& chan, 
	                     const std::string& message);
	void direct_message(const std::string& message);
	void notice(const std::string& notice);
	
	/**
	 * @brief connect to the on_channel_message signal
	 *
	 * @param f the function to callback.
	 *
	 * This signal is triggered when ever the user has sent a channel a 
	 * private message, the format of the callback is:
	 *
	 * @code void f(irc::channel&, irc::user&, const std::string& msg) @endcode
	 *
	 * @return the connection object to disconnect from the signal
	 */
	template<typename F>
	bsig::connection connect_on_channel_message_impl(F&& f);
	/**
	 * @brief connect to the on_direct_message signal
	 *
	 * @param f the function to callback.
	 *
	 * this signal is triggered when ever the user has sent a private message
	 * directly to you
	 *
	 * @code void f(irc::user&, const std::string& msg) \endcode
	 *
	 * @return the connection object to disconnect from the signal
	 */
	template<typename F>
	bsig::connection connect_on_direct_message_impl(F&& f);
	/**
	 * @brief connect to the on_nick_change signal
	 *
	 * @param f the function to callback.
	 *
	 * this signal is triggered when ever this users nick has been changed
	 *
	 * @code void f(irc::user&, const std::string& new_nick) \endcode
	 *
	 * @return the connection object to disconnect from the signal
	 */	
	template<typename F>
	bsig::connection connect_on_nick_change_impl(F&& f);
	/**
	 * @brief connect to the on_notice signal
	 *
	 * @param f the function to callback.
	 *
	 * this signal is triggered when ever this users nick has sent you a notice
	 *
	 * @code void f(irc::user&, const std::string& notice) \endcode
	 *
	 * @return the connection object to disconnect from the signal
	 */
	template<typename F>
	bsig::connection connect_on_notice_impl(F&& f);
	/**
	 * Connect to the on_set_change signal.
	 * This signal is triggered when user's modes changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::user& usr, const prefix& pfx, 
	 *              const irc::mode_diff& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_mode_change_impl(F&& f);
}; //class user_impl

template<typename F>
bsig::connection user_impl::connect_on_channel_message_impl(F&& f) {
	return on_channel_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user_impl::connect_on_direct_message_impl(F&& f) {
	return on_direct_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user_impl::connect_on_nick_change_impl(F&& f) {
	return on_nick_change.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user_impl::connect_on_notice_impl(F&& f) {
	return on_notice.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user_impl::connect_on_mode_change_impl(F&& f) {
	return modes_.connect_on_mode_change(
		[=](const prefix& pfx, const mode_diff& md) {
			f(*this, pfx, md);
		}
	);
}

} //namespace irc

#endif //IRC_USER_HPP
