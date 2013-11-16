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
void channel::add_user(const shared_user& user) {
	assert(user);
	bool success;
	std::tie(std::ignore, success)=users.insert(user);
	if(success) { //if the user is new...
		on_user_join(*this, *user);
	}
}

/*
** User interface
*/
session&       channel::get_session()       { return session_; }
const session& channel::get_session() const { return session_; }


const std::string& channel::get_name() const {
	return name;
}

const std::string& channel::get_topic() const {
	return name;
}

void channel::async_send_message(const std::string& msg) {
	session_.async_privmsg(get_name(), msg);
	on_message(*this, session_.get_self(), msg);
}

void channel::async_part() { session_.async_part(*this); }

channel::user_iterator channel::user_begin() {
	return boost::make_transform_iterator(begin(users), deref);
}
channel::user_iterator channel::user_end() {
	return boost::make_transform_iterator(end(users), deref);
}
channel::const_user_iterator channel::user_begin() const {
	return boost::make_transform_iterator(users.cbegin(), cderef);
}
channel::const_user_iterator channel::user_end() const {
	return boost::make_transform_iterator(users.cend(), cderef);
}

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
		users.erase(it);
		on_user_part(*this, *user, msg);
	} //else was never actually regestered..
}

void channel::part() {
	on_channel_part(*this);
	//DISCONNECT slots?
}

void channel::user_join(const shared_user& user) {
	assert(user);
	add_user(user);
}

void channel::user_quit(const shared_user& user, const std::string& msg) {
	auto it=users.find(user);	
	if(it!=users.cend()) {
		on_user_part(*this, **it, msg);
		users.erase(it);
	}
}

void channel::set_topic(std::string topic_) {
	topic=std::move(topic_);
	on_topic_change(*this, topic);
}

} //namepsace irc


