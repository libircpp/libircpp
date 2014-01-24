
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "deref.hpp"
#include "types.hpp"
#include "modes.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <set>
#include <map>
#include <string>

namespace irc {
/**
    IRC channel class.
*/
class channel {
public:
	using user_container      =std::set<shared_user>;
	using user_iterator       =boost::transform_iterator<deref, user_container::iterator>;
	using const_user_iterator =boost::transform_iterator<deref, user_container::const_iterator>;
private:
	session&              session_;
	std::string           name;
	std::string           topic;
	user_container        users;
	std::set<const user*> operators;

	mode_block            modes;

	//WTF doesn't this work?
	//std::set<std::reference_wrapper<const user>> operators;
//signals
	sig_ch        on_channel_part;
	sig_ch_usr_s  on_message;
	sig_ch_s      on_topic_change;
	sig_ch_usr    on_user_join;
	sig_ch_usr_os on_user_part;
	sig_v         on_list_users;
//deleted functions
	channel(const channel&)           =delete;
	channel(channel&&)                =delete;
	channel& operator=(const channel&)=delete;
	channel& operator=(channel&&)     =delete;

	//helpers
	bool is_nick_in_channel(const std::string& nick) const;

public:
	/**
	 * Constructor.
	 * @param connection__ The associated IRC session.
	 * @param name_        The channel name.
	 */
	channel(session& connection__, std::string name_);
	/**
	 * Returns the associated session object.
	 * @return The associated session object.
	 */
	session& get_session();
	/**
	 * Returns the associated session object (const).
	 * @return The associated session object.
	 * @see get_session()
	 */
	const session& get_session() const;

	//USER INTERFACE
	/**
	 * Returns the channel name.
	 * @return The channel name.
	 */
	const std::string& get_name()  const;
	/**
	 * Returns the channel topic.
	 * @return The channel topic.
	 */
	const std::string& get_topic() const;
	/**
	 * Returns if the specified user is a channel operator.
	 * @param u The user to check.
	 * @return @true if @p u is an operator, @false otherwise.
	 */
	bool is_operator(const user& u) const;
	/**
	 * Sends a message in to the channel.
	 * @param msg The message to send.
	 */
	void async_send_message(const std::string& msg);
	/**
	 * Leaves the channel.
	 */
	void async_part();
	/**
	 * Returns an iterator to the beginning of the user list.
	 * @return An iterator to the beginning of the user list.
	 */
	user_iterator begin_users();
	/**
	 * Returns an iterator to the end of the user list.
	 * @return An iterator to the end of the user list.
	 */
	user_iterator end_users();
	/**
	 * Returns a const iterator to the beginning of the user list.
	 * @return A const iterator to the beginning of the user list.
	 * @see begin_users()
	 */
	const_user_iterator begin_users() const;
	/**
	 * Returns a const iterator to the end of the user list.
	 * @return A const iterator to the end of the user list.
	 * @see end_users()
	 */
	const_user_iterator end_users()   const;
	/**
	 * Returns the mode block for this channel.
	 * @return the mode block for this channel.
	 */
	const mode_block& get_modes() const;
	/**
	 * Returns the mode block for this channel (const version).
	 * @return the mode block for this channel.
	 * @see get_modes()
	 */
	mode_block& get_modes();

	//SYSTEM INTERFACE
	/**
	 * Adds the specified modes to the channel.
	 * @param pfx   The user prefix who sets the new modes.
	 * @param modes The new modes.
	 */
	void apply_mode_diff(const prefix& pfx, mode_diff modes);
	/**
	 * Sends a message to the channel.
	 * @param user    The user who sent the message.
	 * @param message The message.
	 */
	void message(const shared_user& user, const std::string message);
	/**
	 * An user has left the channel.
	 * @param user The user who leaves the channel.
	 * @param msg  A message to display when the user leave the channel (optional).
	 */
	void user_part(const shared_user& user, const optional_string& msg);
	/**
	 * An user has joined the channel.
	 * @param user The new user entry.
	 */
	void user_join(const shared_user& user);
	/**
	 * Adds an user to the channel user list.
	 * @param user The new user entry.
	 * @return @true on success, @false otherwise.
	 */
	bool add_user(const shared_user& user);
	/**
	 * An user has quitted IRC.
	 * @param user The user that has quitted.
	 * @param msg  A quit message (optional).
	 */
	void user_quit(const shared_user& user, const std::string& msg);
	/**
	 * Sets the channel topic.
	 * @param str The new channel topic.
	 */
	void set_topic(std::string str); 
	/**
	 * Sets a specified user as channel operator.
	 * @param u The user to set as op.
	 */
	void set_operator(const user& u);
	/**
	 * Sends a command::list request to the IRC server.
	 */
	void list_users(); //prompts observers to list users
	/**
	 * Sends a command::part request to the IRC server to leave this channel.
	 */
	void part();
	/**
	 * Connect to the on_message signal.
	 * This signal is triggered when a message was sent to the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(channel& chan, user& usr, const std::string& msg) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_message(F&& f);
	/**
	 * Connect to the on_topic signal.
	 * This signal is triggered when the topic has changed.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(channel& chan, const std::string& new_topic) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_topic_change(F&& f);
	/**
	 * Connect to the on_user_join signal.
	 * This signal is triggered when an user joins the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(channel& chan, user& usr) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_join(F&& f);
	/**
	 * Connect to the on_user_join signal.
	 * This signal is triggered when an user leaves the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(channel& chan, user& usr, const optional_string& msg) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_part(F&& f);
	/**
	 * Connect to the on_channel_part signal.
	 * This signal is triggered when we leaves the channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(channel& chan) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_channel_part(F&& f);
	/**
	 * Connect to the on_list_users signal.
	 * This signal is triggered when we required a command::list.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f() @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_list_users(F&& f);
	/**
	 * Connect to the on_set_mode signal.
	 * This signal is triggered when channel has modes set
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& ch, const irc::prefix& pfx, 
	 *              const irc::mode_diff& modes)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_mode_change(F&& f);
}; //class channel


// Template impl
template<typename F>
bsig::connection channel::connect_on_message(F&& f) {
	return on_message.connect(std::forward<F>(f));	
}
template<typename F>
bsig::connection channel::connect_on_user_join(F&& f) {
	return on_user_join.connect(std::forward<F>(f));	
}
template<typename F>
bsig::connection channel::connect_on_user_part(F&& f) {
	return on_user_part.connect(std::forward<F>(f));	
}
template<typename F>
bsig::connection channel::connect_on_channel_part(F&& f) {
	return on_channel_part.connect(std::forward<F>(f));	
}
template<typename F>
bsig::connection channel::connect_on_topic_change(F&& f) {
	return on_topic_change.connect(std::forward<F>(f));	
}
template<typename F>
bsig::connection channel::connect_on_list_users(F&& f) {
	return on_list_users.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection channel::connect_on_mode_change(F&& f) {
	return modes.connect_on_mode_change(
		[=](const prefix& pfx, const mode_diff& md) {
			f(*this, pfx, md);
		}
	);
}

} //namespace irc

#endif //CHANNEL_HPP
