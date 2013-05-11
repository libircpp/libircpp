#include "channel.hpp"

#include <utility>

namespace irc {

channel::channel(std::string name_)
:	name { std::move(name_) }
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
const std::string& channel::get_name() const {
	return name;
}

const std::string& channel::get_topic() const {
	return name;
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


