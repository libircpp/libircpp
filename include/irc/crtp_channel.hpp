#ifndef CRTP_CHANNEL_HPP
#define CRTP_CHANNEL_HPP

#include "types.hpp"

#include <string>

namespace irc {

template<typename T> struct channel_traits;

/**
 * This class serves as MASK to limit the interface seen by the user so that
 * they can not call the system interface used by session and so forth
 */
template<typename ImplType>
class crtp_channel {
protected:
	crtp_channel()=default;
public:
	using user_iterator      =typename channel_traits<ImplType>::user_iterator;
	using const_user_iterator=typename channel_traits<ImplType>::const_user_iterator;

	/*
	 * Gets the session from which this channel belongs
	 * @return the session object
	 */
	session& get_session();
	/*
	 * Gets the const session from which this cosnt channel belongs
	 * @return the const session object
	 */
	const session& get_session() const;
	/**
	 * Get the name of the channel
	 * @return the name of the channel
	 */
	const std::string& get_name() const; 
	/**
	 * Get mode object
	 *
	 * @return returns the modes belonging to the channel
	 */
	const mode_block& get_modes() const;
	/**
	 * Get mode object
	 *
	 * @return returns the const modes object belonging to the const channel
	 */
	mode_block& get_modes();
	/**
	 * Send a private message to channel
	 * Note: this is asynchronous
	 *
	 * @param msg the message you wish to send to this channel
	 * @throws if session.get_connection().is_ready() or session.is_active() is false
	 */
	void send_privmsg(const std::string& msg);

	/**
	 * Request to change the channel topic
	 * @param str The new channel topic requested
	 * @throws if session.get_connection().is_ready() or session.is_active() is false
	 */
	void change_topic(const std::string& str); 

	/**
	 * Requests to leave the channel
	 * Note: this is asynchronous
	 *
	 * @throws if session.get_connection().is_ready() or session.is_active() is false
	 */
	void part();
	
	/**
	 * Find a user in this channel by their nick
	 * @return a channel_iterator to the user, 
	 * if the user is not in the list then the user_end() will be returned
	 */
	user_iterator find_user(const std::string& nick);
	/**
	 * Find a user in this channel by their nick
	 * @return a channel_iterator to the user, 
	 * if the user is not in the list then the user_end() will be returned
	 */
	const_user_iterator find_user(const std::string& nick) const;
	/**
	 * Returns an channel_iterator to the beginning of the user list.
	 * @return An channel_iterator to the beginning of the user list.
	 */
	user_iterator begin_users();
	/**
	 * Returns a const channel_iterator to the beginning of the user list.
	 * @return A const channel_iterator to the beginning of the user list.
	 * @see begin_users()
	 */
	user_iterator end_users();
	/**
	 * Returns a const channel_iterator to the beginning of the user list.
	 * @return A const channel_iterator to the beginning of the user list.
	 * @see begin_users()
	 */
	const_user_iterator begin_users() const;
	/**
	 * Returns a const user_iterator to the end of the user list.
	 * @return A const user_iterator to the end of the user list.
	 * @see end_users()
	 */
	const_user_iterator end_users()   const;

	/**
	 * Connect to the on_user_join signal.
	 * This signal is triggered when an user joins the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& chan, irc::user& usr) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_join(F&& f);
	/**
	 * Connect to the on_user_join signal.
	 * This signal is triggered when an user leaves the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code 
	 * void f(irc::channel& chan, 
	 *        irc::user& usr, 
	 *        const std::string& msg) 
	 * @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_part(F&& f);
	/**
	 * Connect to the on_user_quit signal.
	 * This signal is triggered when an user in this channel quit the IRC server
	 * @note if a user is in multiple channels, and quits, then this signal will
	 * be triggered once for each signal
	 *
	 * @param f A callback function with the following signature:
	 * @code 
	 * void f(irc::channel& chan, 
	 *        irc::user& usr, 
	 *        const std::string& msg) 
	 * @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_quit(F&& f);
	/**
	 * Connect to the on_privmsg signal.
	 * This signal is triggered when a PRIVMSG was sent to the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code 
	 * void f(irc::channel& chan,
	 *        irc::user& usr, 
	 *        const std::string& msg) 
	 * @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_privmsg(F&& f);
	/**
	 * Connect to the on_topic signal.
	 * This signal is triggered when the topic has changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& chan, const std::string& new_topic) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_topic_change(F&& f);
	/**
	 * Connect to the on_mode_change signal.
	 * This signal is triggered when channel has modes have been change
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& ch, 
	 *              const irc::prefix& pfx, 
	 *              const irc::mode_diff& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_mode_change(F&& f);
	/**
	 * Connect to the on_user_mode_change signal.
	 * This signal is triggered when the mode for a user in channel
	 * get change, such as adding voice +v and opping +o
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& ch, 
	 *              irc::user& usr, 
	 *              const irc::prefix& pfx,
	 *              const irc::mode_diff& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_mode_change(F&& f);
}; //class crtp_channel

//HELPERS
template<typename ImplType>
ImplType& get_impl(crtp_channel<ImplType>& chan) {
	return static_cast<ImplType&>(chan);
}
template<typename ImplType>
const ImplType& get_impl(const crtp_channel<ImplType>& chan) {
	return static_cast<const ImplType&>(chan);
}

//ACCESSORS
template<typename ImplType>
session& crtp_channel<ImplType>::get_session() {
	return get_impl(*this).get_session();
}
template<typename ImplType>
const session& crtp_channel<ImplType>::get_session() const {
	return get_impl(*this).get_session();
}
template<typename ImplType>
const std::string& crtp_channel<ImplType>::get_name() const {
	return get_impl(*this).get_name();
}
template<typename ImplType>
const mode_block& crtp_channel<ImplType>::get_modes() const {
	return get_impl(*this).get_modes();
}
template<typename ImplType>
mode_block& crtp_channel<ImplType>::get_modes() {
	return get_impl(*this).get_modes();
}

template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator 
crtp_channel<ImplType>::find_user(const std::string& nick) {
	return get_impl(*this).find_user(nick);
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::find_user(const std::string& nick) const {
	return get_impl(*this).find_user(nick);
}
template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator
crtp_channel<ImplType>::begin_users() {
	return get_impl(*this).begin_users();
}
template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator
crtp_channel<ImplType>::end_users() {
	return get_impl(*this).end_users();
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::begin_users() const {
	return get_impl(*this).begin_users();
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::end_users() const {
	return get_impl(*this).end_users();
}

//ASYNC API INTERACTION
template<typename ImplType>
void crtp_channel<ImplType>::send_privmsg(const std::string& msg) {
	get_impl(*this).send_privmsg(msg);
}
template<typename ImplType>
void crtp_channel<ImplType>::change_topic(const std::string& str) {
	get_impl(*this).change_topic(str);
}
template<typename ImplType>
void crtp_channel<ImplType>::part() {
	get_impl(*this).part();
}

//SIGNAL REGISTRATION
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_join(F&& f) {
	return get_impl(*this).connect_on_user_join(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_part(F&& f) {
	return get_impl(*this).connect_on_user_part(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_quit(F&& f) {
	return get_impl(*this).connect_on_user_quit(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_privmsg(F&& f) {
	return get_impl(*this).connect_on_privmsg(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_topic_change(F&& f) {
	return get_impl(*this).connect_on_topic_change(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_mode_change(F&& f) {
	return get_impl(*this).connect_on_mode_change(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_mode_change(F&& f) {
	return get_impl(*this).connect_on_user_mode_change(std::forward<F>(f));
}

} //namespace irc

#endif //IRC_CRTP_CHANNEL_HPP

/*
#ifndef IRC_CHANNEL_HPP
#define IRC_CHANNEL_HPP

#include "impl_channel.hpp"
#include "crtp_channel.hpp"

using channel=crtp_channel<impl_channel>;

#endif IRC_CHANNEL_HPP
*/
