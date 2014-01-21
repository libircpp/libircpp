
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef USER_HPP
#define USER_HPP

#include "types.hpp"
#include "modes.hpp"
#include "prefix.hpp"

#include <string>

namespace irc {

/**
 * The user class models and IRC user
 */
class user {
	std::string  nick;
	prefix       pfx;
	mode_block   modes;
//signals
	sig_ch_usr_s on_channel_message;
	sig_usr_s    on_direct_message;
	sig_usr_s    on_nick_change;
	sig_usr_s    on_notice;

//deleted functions
	user(const user&)           =delete;
	user(user&&)                =delete;
	user& operator=(const user&)=delete;
	user& operator=(user&&)     =delete;
public:
	/**
	 * Constructor.
	 * @param nick_ The user nickname.
	 */
	user(std::string nick_);
	/**
	 * Constructor.
	 * @param nick_ The user nickname.
	 * @param pfx_  The user hostmask.
	 */
	user(std::string nick_, prefix pfx_);

	//USER INTERFACE
	/**
	 * @brief returns the nick for this user
	 * @return the name
	 */
	const std::string& get_nick()    const;
	/**
	 * @brief returns the prefix for this user
	 * @return the prefix
	 */
	const prefix&      get_prefix()  const;
	/**
	 * @brief returns the mode block for this user
	 * @return the mode block
	 */
	mode_block&        get_modes();
	/**
	 * @brief const version
	 * @return the mode block
	 * @see get_modes
	 */
	const mode_block & get_modes() const;

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
	bsig::connection connect_on_channel_message(F&& f);
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
	bsig::connection connect_on_direct_message(F&& f);
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
	bsig::connection connect_on_nick_change(F&& f);
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
	bsig::connection connect_on_notice(F&& f);
	/**
	 * Connect to the on_set_mode signal.
	 * This signal is triggered when user modes changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::user& usr, const prefix& pfx, 
	 *              const std::vector<std::pair<char, optional_string>>& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_set_mode(F&& f);
	/**
	 * Connect to the on_set_mode signal.
	 * This signal is triggered when user modes changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::user& usr, const prefix& pfx, 
	 *              const std::vector<std::pair<char, optional_string>>& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_unset_mode(F&& f);
}; //class user

template<typename F>
bsig::connection user::connect_on_channel_message(F&& f) {
	return on_channel_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_direct_message(F&& f) {
	return on_direct_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_nick_change(F&& f) {
	return on_nick_change.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_notice(F&& f) {
	return on_notice.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_set_mode(F&& f) {
	return modes.connect_on_set_mode(
		[=](const prefix& pfx, const mode_list& ml) {
			f(*this, pfx, ml);
		}
	);
}
template<typename F>
bsig::connection user::connect_on_unset_mode(F&& f) {
	return modes.connect_on_unset_mode(
		[=](const prefix& pfx, const mode_list& ml) {
			f(*this, pfx, ml);
		}
	);
}

} //namespace irc

#endif //USER_HPP
