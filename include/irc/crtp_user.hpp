
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_CRTP_USER_HPP
#define IRC_CRTP_USER_HPP

#include <string>
#include "types.hpp"

namespace irc {

//for now this will not be crtp but virtual
template<typename ImplType>
class crtp_user {
	crtp_user(const crtp_user&)           =delete;
	crtp_user(crtp_user&&)                =delete;
	crtp_user& operator=(const crtp_user&)=delete;
	crtp_user& operator=(crtp_user&&)     =delete;
protected:
	crtp_user()                           =default;
public:
	/**
	 * @brief returns the prefix for this user
	 * @return the prefix
	 */
	const std::string& get_nick() const;
	/**
	 * @brief returns the prefix for this user
	 * @return the prefix
	 */
	const prefix& get_prefix()  const;
	/**
	 * @brief returns the mode block for this user
	 * @return the mode block
	 */
	mode_block& get_modes();
	/**
	 * @brief const version
	 * @return the mode block
	 * @see get_modes
	 */
	const mode_block & get_modes() const;
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
	 * Connect to the on_set_change signal.
	 * This signal is triggered when user's modes changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::user& usr, const prefix& pfx, 
	 *              const irc::mode_diff& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_mode_change(F&& f);

}; //class crtp_user

//HELPERS
template<typename ImplType>
ImplType& get_impl(crtp_user<ImplType>& chan) {
	return static_cast<ImplType&>(chan);
}
template<typename ImplType>
const ImplType& get_impl(const crtp_user<ImplType>& chan) {
	return static_cast<const ImplType&>(chan);
}

//ACCESSORS
template<typename ImplType>
const std::string& crtp_user<ImplType>::get_nick() const {
	return get_impl(*this).get_nick_impl();
}
template<typename ImplType>
const prefix& crtp_user<ImplType>::get_prefix() const {
	return get_impl(*this).get_prefix_impl();
}
template<typename ImplType>
const mode_block& crtp_user<ImplType>::get_modes() const {
	return get_impl(*this).get_modes_impl();
}
template<typename ImplType>
mode_block& crtp_user<ImplType>::get_modes() {
	return get_impl(*this).get_modes_impl();
}

//SIGNALS
template<typename ImplType>
template<typename F>
bsig::connection crtp_user<ImplType>::connect_on_channel_message(F&& f) {
	return get_impl(*this).connect_on_channel_message_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F>
bsig::connection crtp_user<ImplType>::connect_on_direct_message(F&& f) {
	return get_impl(*this).connect_on_direct_message_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F>
bsig::connection crtp_user<ImplType>::connect_on_nick_change(F&& f) {
	return get_impl(*this).connect_on_nick_change_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F>
bsig::connection crtp_user<ImplType>::connect_on_notice(F&& f) {
	return get_impl(*this).connect_on_notice_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F>
bsig::connection crtp_user<ImplType>::connect_on_mode_change(F&& f) {
	return get_impl(*this).connect_on_mode_change_impl(std::forward<F>(f));
}

} //namespace irc

#endif //IRC_CRPT_USER_HPP
