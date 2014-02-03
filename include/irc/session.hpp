
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SESSION_HPP
#define SESSION_HPP

#include "command.hpp"
#include "deref.hpp"
#include "types.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <memory> //shared_ptr
#include <string>
#include <vector>
#include <unordered_map>

namespace irc {

bool is_channel(const std::string& s);
bool is_operator(const std::string& s);
/**
    IRC session class.
*/
class session {
//member types
	using channel_container                 =std::unordered_map<std::string, shared_channel>;
	using channel_iterator                  =channel_container::iterator;

	using user_container                    =std::unordered_map<std::string, shared_user>;
	using user_iterator                     =user_container::iterator;

	using const_user_iterator               =boost::transform_iterator<
												second_deref,
												user_container::const_iterator
											>;
	using const_channel_iterator            =boost::transform_iterator<
												second_deref,
												channel_container::const_iterator
											>;
//member variables
	std::unique_ptr<persistant_connection>   connection_;
	channel_container                        channels_;
	user_container                           users_;
	std::string                              nickname_, username_, realname_, motd_;
//callback
	sig_s                                    on_motd;
	sig_ch                                   on_join_channel;
	sig_s                                    on_notice;
	sig_usr_s                                on_user_notice;
	sig_usr                                  on_new_user;
	sig_s                                    on_irc_error;
	sig_s                                    on_protocol_error;
//helper
	void prepare_connection();
	channel_iterator create_new_channel(const std::string& channel_name);
	channel_iterator get_or_create_channel(const std::string& channel_name);

	user_iterator create_new_user(const std::string& username,
	                              const prefix& pfx);
	user_iterator get_or_create_user(const prefix& pfx);
	user_iterator get_or_create_user(const std::string& nickname);
//handlers
	void handle_privmsg(const prefix&                   pfx,
	                    const std::string&              target,
	                    const std::string&              content);

	void handle_notice (const prefix&                   pfx,
	                    const std::string&              nickname,
	                    const std::string&              msg);

	void handle_ping(   const prefix&                   pfx,
	                    const std::string&              channel_name,
	                    const optional_string&          msg);

	void handle_topic(  const std::string&              channel_name,
	                    std::string                     topic);

	void handle_join(   const prefix&                   pfx,
	                    const std::string&              channel_name);

	void handle_part(   const prefix&                   pfx,
	                    const std::string&              channel_name,
	                    const optional_string&          msg);

	void handle_quit(   const prefix&                   pfx,
	                    const std::string&              channel_name);

	void handle_reply(  const prefix&                   pfx,
	                    command                         cmd,
	                    const std::vector<std::string>& params);

	void handle_mode(   const prefix&                   pfx,
	                    const std::string&              agent,
	                    const std::string&              mode);
//deleted functions
	session(const session&)           =delete;
	session(session&&)                =delete;
	session& operator=(const session&)=delete;
	session& operator=(session&&)     =delete;
public:
	/**
	 * Constructor.
	 * @param conn      An enstablished IRC connection.
	 * @param nickname  A nickname.
	 * @param username  An user name.
	 * @param realname  A real, full user name.
	 */
	session(std::unqiue_ptr<persistant_connection>&& conn,
	        std::string nickname, std::string username, std::string realname);


	//TODO
	//session(std::string hostname, std::string service,
	//       std::string nickname, std::string username, std::string realname);


	/**
	 * Returns the user nick name.
	 * @return The user nick name.
	 */
	const std::string& get_nick() const;
	/**
	 * Returns a reference to our current user details.
	 * @return A reference to our current user details.
	 */
	user& get_self();
	/**
	 * Returns a const reference to our current user details.
	 * @return A const reference to our current user details.
	 * @see get_self()
	 */
	const user& get_self() const;
	/**
	 * Returns a const iterator to the beginning of the user list.
	 * @return A const iterator to the beginning of the user list.
	 */
	const_user_iterator begin_users() const;
	/**
	 * Returns a const iterator to the end of the user list.
	 * @return A const iterator to the end of the user list.
	 */
	const_user_iterator end_users()   const;
	/**
	 * Returns a const iterator to the beginning of the channel list.
	 * @return A const iterator to the beginning of the channel list.
	 */
	const_channel_iterator channel_begin() const;
	/**
	 * Returns a const iterator to the end of the channel list.
	 * @return A const iterator to the end of the channel list.
	 */
	const_channel_iterator channel_end()   const;

//async interface
	/**
	 * Joins the specified channel.
	 * @param channel_name The channel to join.
	 */
	void async_join(const std::string& channel_name);
	/**
	 * Leaves the specified channel.
	 * @param chan The channel to leave.
	 */
	void async_part(const channel& chan);
	/**
	 * Sends a message to the specified target.
	 * @param target The target where to send the message, a channel or a nickname.
	 * @param msg    The message to send.
	 */
	void async_privmsg(const std::string& target, const std::string& msg);
	/**
	 * Changes our current nickname.
	 * @param target The new nick name.
	 */
	void async_change_nick(const std::string& target);
	/**
	 * Closes the connection.
	 */
	void stop(); //async_stop?
	/**
	 * Connect to the on_motd signal.
	 * This signal is triggered when receiving the Message Of The Day
	 * from the IRC server.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(std::string msg) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_motd(F&& f);
	/**
	 * Connect to the on_join_channel signal.
	 * This signal is triggered when an user joins an IRC channel.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(irc::channel& chan) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_join_channel(F&& f);
	/**
	 * Connect to the on_notice signal.
	 * This signal is triggered when an irc::command::notice was sent.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(std::string msg) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_notice(F&& f);
	/**
	 * Connect to the on_user_notice signal.
	 * This signal is triggered when an irc::command::notice was sent
	 * to an user.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(user& u, const std::string& msg) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_user_notice(F&& f);
	/**
	 * Connect to the on_new_user signal
	 * this signal is triggered when ever a new user is "known" to the system
	 * either because they've joined a channel or sent you a privmsg or
	 * other mechanism
	 *
	 * This is useful if you wish to add hooks to ALL users, such as
	 * if you want to be able to capture any nick change you might do:
	 *
	 * @code session.connect_on_new_user([](irc::user& u) {
	 *		u.connect_on_nick_change(my_nick_change_handler);
	 * 	}
	 * );
	 * @endcode
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(user& u) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_new_user(F&& f);
	/**
	 * Connect to the on_irc_error
	 * This signal is triggered when ever the server has replied
	 * with an error message
	 *
	 * This is NOT:
	 * 	- A failure with the connection
	 * 	- A failure to parse a message
	 * 	- An interal error with the library (algorithm failure and so on)
	 *
	 * In the IRC RFC with a numeric replies beginning with ERR_ or
	 * the ERROR: reply
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(user& u) @endcode
	 *
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_irc_error(F&& f);
}; //class session


template<typename F>
bsig::connection session::connect_on_motd(F&& f) {
	return on_motd.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection session::connect_on_join_channel(F&& f) {
	return on_join_channel.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection session::connect_on_notice(F&& f) {
	return on_notice.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection session::connect_on_user_notice(F&& f) {
	return on_user_notice.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection session::connect_on_new_user(F&& f) {
	return on_new_user.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection session::connect_on_irc_error(F&& f) {
	return on_irc_error.connect(std::forward<F>(f));
}

} //namespace irc

#endif //SESSION_HPP

