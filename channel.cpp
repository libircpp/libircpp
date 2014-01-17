
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "channel.hpp"
#include "session.hpp"

#include <utility>

namespace irc {

channel::channel(session& session__, std::string name_)
:	session_ ( session__        )
,	name     { std::move(name_) }
{	}


/*
** Internal
*/
bool channel::add_user(const shared_user& user) {
	assert(user);
	bool success;
	std::tie(std::ignore, success)=users.insert(user);
	return success;
}

/*
** User interface
*/
session&       channel::get_session()       { return session_; }
const session& channel::get_session() const { return session_; }

const std::string& channel::get_name()   const { return name; }
const std::string& channel::get_topic()  const { return name; }
bool channel::is_operator(const user& u) const { return operators.count(&u); }

void channel::async_send_message(const std::string& msg) {
	session_.async_privmsg(get_name(), msg);
	on_message(*this, session_.get_self(), msg);
}

void channel::async_part() { session_.async_part(*this); }

channel::user_iterator channel::begin_users() {
	return boost::make_transform_iterator(begin(users), deref{});
}
channel::user_iterator channel::end_users() {
	return boost::make_transform_iterator(end(users), deref{});
}
channel::const_user_iterator channel::begin_users() const {
	return boost::make_transform_iterator(users.cbegin(), deref{});
}
channel::const_user_iterator channel::end_users() const {
	return boost::make_transform_iterator(users.cend(), deref{});
}


const mode_block& channel::get_modes() const { return modes; }
mode_block& channel::get_modes() { return modes; }

/*
** System interface
*/
void channel::message(const shared_user& user, const std::string message) {
	assert(user);
	add_user(user);
	on_message(*this, *user, message);
}

void channel::user_part(const shared_user& user, const optional_string& msg) {
	assert(user);
	auto it=users.find(user);
	if(it!=users.cend()) {
		on_user_part(*this, *user, msg);
		operators.erase(user.get());
		users.erase(it);
	} //else was never actually regestered..
}

void channel::part() {
	on_channel_part(*this);
	//DISCONNECT slots?
}

void channel::user_join(const shared_user& user) {
	assert(user);
	bool success=add_user(user);
	if(success) { //if the user is new...
		on_user_join(*this, *user);
	}
}

void channel::user_quit(const shared_user& user, const std::string& msg) {
	auto it=users.find(user);	
	if(it!=users.cend()) {
		on_user_part(*this, **it, msg);
		operators.erase(user.get());
		users.erase(it);
	}
}

void channel::set_topic(std::string topic_) {
	topic=std::move(topic_);
	on_topic_change(*this, topic);
}

void channel::set_operator(const user& u) {
	operators.insert(&u);
}

void channel::list_users() {
	on_list_users();
}

} //namepsace irc


