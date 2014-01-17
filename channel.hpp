
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

class channel {

	using user_container      =std::set<shared_user>;
	using user_iterator       =boost::transform_iterator<deref, user_container::iterator>;
	using const_user_iterator =boost::transform_iterator<deref, user_container::const_iterator>;

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
public:
	channel(session& connection__, std::string name_);

	session&       get_session();
	const session& get_session() const;

	//USER INTERFACE
	const std::string& get_name()  const;
	const std::string& get_topic() const;

	bool is_operator(const user& u) const;

	void async_send_message(const std::string& msg);
	void async_part();

	user_iterator       begin_users();
	user_iterator       end_users();
	const_user_iterator begin_users() const;
	const_user_iterator end_users()   const;

	const mode_block& get_modes() const;
	mode_block& get_modes();

	//SYSTEM INTERFACE
	void message(const shared_user& user, const std::string message);
	void user_part(const shared_user& user, const optional_string& msg);
	void user_join(const shared_user& user);
	bool add_user(const shared_user& user);
	void user_quit(const shared_user& user, const std::string& msg);
	void set_topic(std::string str); 
	void set_operator(const user& u);
	void list_users(); //prompts observers to list users
	void part();

	template<typename F> bsig::connection connect_on_message(F&& f);
	template<typename F> bsig::connection connect_on_topic_change(F&& f);
	template<typename F> bsig::connection connect_on_user_join(F&& f);
	template<typename F> bsig::connection connect_on_user_part(F&& f);
	template<typename F> bsig::connection connect_on_channel_part(F&& f);
	template<typename F> bsig::connection connect_on_list_users(F&& f);
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

} //namespace irc

#endif //CHANNEL_HPP
