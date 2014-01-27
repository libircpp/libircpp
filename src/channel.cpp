
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "channel.hpp"
#include "user.hpp"
#include "session.hpp"

#include "util.hpp"

#include <utility>

namespace irc {

channel_impl::channel_impl(session& session__, std::string name__)
:	session_ ( session__        )
,	name_    { std::move(name__) }
{	}


bool channel_impl::is_nick_in_channel(const std::string& nick) const {
	return std::find_if(begin_users(), end_users(), [&](const irc::user& u) { 
		return u.get_nick() == nick; }) == end_users();
}

/*
** Internal
*/
bool channel_impl::add_user(const shared_user& user) {
	assert(user);
	bool success;
	std::tie(std::ignore, success)=users_.insert(user);
	return success;
}

/*
** User interface
*/
session&       channel_impl::get_session_impl()       { return session_; }
const session& channel_impl::get_session_impl() const { return session_; }

const std::string& channel_impl::get_name_impl()   const { return name_; }
const std::string& channel_impl::get_topic_impl()  const { return name_; }

bool channel_impl::is_operator(const user& u) const { return false; }

void channel_impl::send_privmsg_impl(const std::string& msg) {
	session_.async_privmsg(get_name(), msg);
	on_message(*this, session_.get_self(), msg);
}

void channel_impl::send_part_impl() { session_.async_part(*this); }

channel_impl::user_iterator channel_impl::begin_users_impl() {
	return boost::make_transform_iterator(begin(users_), deref{});
}
channel_impl::user_iterator channel_impl::end_users_impl() {
	return boost::make_transform_iterator(end(users_), deref{});
}
channel_impl::const_user_iterator channel_impl::begin_users_impl() const {
	return boost::make_transform_iterator(users_.cbegin(), deref{});
}
channel_impl::const_user_iterator channel_impl::end_users_impl() const {
	return boost::make_transform_iterator(users_.cend(), deref{});
}


const mode_block& channel_impl::get_modes_impl() const { return modes_; }
mode_block& channel_impl::get_modes_impl() { return modes_; }

/*
** System interface
*/
void channel_impl::apply_mode_diff(const prefix& pfx, mode_diff md) {
	mode_list user_modes;
	auto& ml=md.modes;

	//separate user@channel modes
	auto p=util::separate(ml.begin(), ml.end(), std::back_inserter(user_modes),
		[&](const mode_block::value_type& v) {
			return v.second && is_nick_in_channel(*v.second); }); 

	ml.erase(p.first, ml.end());

	modes_.apply_mode_diff(pfx, md);
	//TODO handle user modes
}

void channel_impl::message(const shared_user& user, const std::string& message) {
	assert(user);
	add_user(user);
	on_message(*this, *user, message);
}

void channel_impl::user_part(const shared_user& user, const optional_string& msg) {
	assert(user);
	auto it=users_.find(user);
	if(it!=users_.cend()) {
		on_user_part(*this, *user, msg);
		users_.erase(it);
	} //else was never actually regestered..
}

void channel_impl::part() {
	on_channel_part(*this);
	//DISCONNECT slots?
}

void channel_impl::user_join(const shared_user& user) {
	assert(user);
	bool success=add_user(user);
	if(success) { //if the user is new...
		on_user_join(*this, *user);
	}
}

void channel_impl::user_quit(const shared_user& user, const std::string& msg) {
	auto it=users_.find(user);	
	if(it!=users_.cend()) {
		on_user_part(*this, **it, msg);
		users_.erase(it);
	}
}

void channel_impl::set_topic(std::string topic) {
	topic_=std::move(topic);
	on_topic_change(*this, topic_);
}

void channel_impl::set_operator(const user& u) {
}

void channel_impl::list_users() {
	on_list_users(*this);
}

} //namepsace irc
