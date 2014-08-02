
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
	crtp_channel(const crtp_channel&)           =delete;
	crtp_channel(crtp_channel&&)                =delete;
	crtp_channel& operator=(const crtp_channel&)=delete;
	crtp_channel& operator=(crtp_channel&&)     =delete;
protected:
	crtp_channel()                              =default;
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
	 * Returns the channel topic.
	 * @return The channel topic.
	 */
	const std::string& get_topic() const;
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
	void send_part(const std::string& msg);
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
	 * Connect to the on_channel_join signal.
	 * This signal is triggered when you part from a channel, usually as
	 * a successful response to issueing the part mesage (chan.part())
	 *
	 * @param f A callback function with the following signature:
	 * @code 
	 * void f(irc::channel& chan)
	 * @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_channel_part(F&& f);
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
	 * Connect to the on_list_users signal.
	 * This signal is triggered when we required a command::list.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_list_users(F&& f);
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
	return get_impl(*this).get_session_impl();
}
template<typename ImplType>
const session& crtp_channel<ImplType>::get_session() const {
	return get_impl(*this).get_session_impl();
}
template<typename ImplType>
const std::string& crtp_channel<ImplType>::get_name() const {
	return get_impl(*this).get_name_impl();
}
template<typename ImplType>
const mode_block& crtp_channel<ImplType>::get_modes() const {
	return get_impl(*this).get_modes_impl();
}
template<typename ImplType>
mode_block& crtp_channel<ImplType>::get_modes() {
	return get_impl(*this).get_modes_impl();
}
template<typename ImplType>
const std::string& crtp_channel<ImplType>::get_topic() const {
	return get_impl(*this).get_topic_impl();
}

template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator 
crtp_channel<ImplType>::find_user(const std::string& nick) {
	return get_impl(*this).find_user_impl(nick);
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::find_user(const std::string& nick) const {
	return get_impl(*this).find_user_impl(nick);
}
template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator
crtp_channel<ImplType>::begin_users() {
	return get_impl(*this).begin_users_impl();
}
template<typename ImplType>
typename crtp_channel<ImplType>::user_iterator
crtp_channel<ImplType>::end_users() {
	return get_impl(*this).end_users_impl();
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::begin_users() const {
	return get_impl(*this).begin_users_impl();
}
template<typename ImplType>
typename crtp_channel<ImplType>::const_user_iterator 
crtp_channel<ImplType>::end_users() const {
	return get_impl(*this).end_users_impl();
}

//ASYNC API INTERACTION
template<typename ImplType>
void crtp_channel<ImplType>::send_privmsg(const std::string& msg) {
	//ImplType::send_privmsg(static_cast<ImplType*>(this), msg);
	get_impl(*this).send_privmsg_impl(msg);
}
template<typename ImplType>
void crtp_channel<ImplType>::change_topic(const std::string& str) {
	get_impl(*this).change_topic_impl(str);
}
template<typename ImplType>
void crtp_channel<ImplType>::send_part(const std::string& msg) {
	get_impl(*this).send_part_impl(msg);
}

//SIGNAL REGISTRATION
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_join(F&& f) {
	return get_impl(*this).connect_on_user_join_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_part(F&& f) {
	return get_impl(*this).connect_on_user_part_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_channel_part(F&& f) {
	return get_impl(*this).connect_on_channel_part_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_quit(F&& f) {
	return get_impl(*this).connect_on_user_quit_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_privmsg(F&& f) {
	return get_impl(*this).connect_on_privmsg_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_topic_change(F&& f) {
	return get_impl(*this).connect_on_topic_change_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_list_users(F&& f) {
	return get_impl(*this).connect_on_list_users_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_mode_change(F&& f) {
	return get_impl(*this).connect_on_mode_change_impl(std::forward<F>(f));
}
template<typename ImplType>
template<typename F> 
bsig::connection crtp_channel<ImplType>::connect_on_user_mode_change(F&& f) {
	return get_impl(*this).connect_on_user_mode_change_impl(std::forward<F>(f));
}

} //namespace irc

#endif //IRC_CRTP_CHANNEL_HPP
