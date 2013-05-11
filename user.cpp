#include "user.hpp"

#include <utility>

namespace irc {

user::user(std::string nick_)
:	nick { std::move(nick_) } 
{	}

user::user(std::string nick_, prefix pfx_)
:	nick { std::move(nick_) } 
,	pfx  { std::move(pfx_)  }
{	}

/*
** User interface
*/
const std::string& user::get_nick() const {
	return nick;
}

const prefix& user::get_prefix() const {
	return pfx;
}

/*
** System interface
*/
void user::set_nick(std::string nick_) {
	nick=std::move(nick_);
	on_nick_change(*this, nick);
}

void user::set_prefix(prefix pfx_) {
	pfx=std::move(pfx_);
}

void user::channel_message(const shared_channel& chan, 
                           const std::string& message) {
	assert(chan);
	on_channel_message(*chan, *this, message);
}

void user::direct_message(const std::string& message) {
	on_direct_message(*this, message);
}


} //namespace irc
