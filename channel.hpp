#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "deref.hpp"
#include "types.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <set>
#include <string>

namespace irc {

class channel {

	using user_container      =std::set<shared_user>;
	using user_iterator       =boost::transform_iterator<deref, user_container::iterator>;
	using const_user_iterator =boost::transform_iterator<deref, user_container::const_iterator>;

	session&              session_;
	std::string           name;
	std::string           topic;
	std::set<shared_user> users;
	std::set<const user*> operators;
	//WTF doesn't this work?
	//std::set<std::reference_wrapper<const user>> operators;
//signals
	sig_ch        on_channel_part;
	sig_ch_usr_s  on_message;
	sig_ch_s      on_topic_change;
	sig_ch_usr    on_user_join;
	sig_ch_usr_os on_user_part;
//helpers
	void add_user(const shared_user& user);
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

	user_iterator       user_begin();
	user_iterator       user_end();
	const_user_iterator user_begin() const;
	const_user_iterator user_end()   const;

	//SYSTEM INTERFACE
	void message(const shared_user& user, const std::string message);
	void user_part(const shared_user& user, const optional_string& msg);
	void user_join(const shared_user& user);
	void user_quit(const shared_user& user, const std::string& msg);
	void set_topic(std::string str); 
	void set_operator(const user& u);
	void part();

	template<typename F>
	bsig::connection connect_on_message(F&& f);
	template<typename F>
	bsig::connection connect_on_topic_change(F&& f);
	template<typename F>
	bsig::connection connect_on_user_join(F&& f);
	template<typename F>
	bsig::connection connect_on_user_part(F&& f);
	template<typename F>
	bsig::connection connect_on_channel_part(F&& f);
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


} //namespace irc

#endif //CHANNEL_HPP
