
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "user.hpp"
#include "channel.hpp"

#include <utility>

namespace irc {

user_impl::user_impl(std::string nick)
:	nick_{ std::move(nick) } 
{	}

user_impl::user_impl(std::string nick, prefix pfx)
:	nick_{ std::move(nick) } 
,	pfx_ { std::move(pfx)  }
{	}

/*
** User interface
*/
const std::string& user_impl::get_nick_impl() const { return nick_; }

const prefix& user_impl::get_prefix_impl() const { return pfx_; }

const mode_block& user_impl::get_modes_impl() const { return modes_; }
mode_block& user_impl::get_modes_impl() { return modes_; }

/*
void user_impl::send_privmsg_impl(const std::string& msg) {
	session_.async_privmsg(get_nick(), msg);
}
*/
/*
void user_impl::send_notice_impl(const std::string& msg) {
	session_.async_notice(get_nick(), msg);
}
*/


/*
** System interface
*/
void user_impl::set_nick(std::string nick) {
	nick_=std::move(nick);
	on_nick_change(*this, nick_);
}

void user_impl::set_prefix(prefix pfx) {
	pfx_=std::move(pfx);
}

void user_impl::channel_message(const shared_channel& chan, 
                           const std::string& message) {
	assert(chan);
	on_channel_message(*chan, *this, message);
}

void user_impl::direct_message(const std::string& message) {
	on_direct_message(*this, message);
}

void user_impl::notice(const std::string& msg) {
	on_notice(*this, msg);
}

} //namespace irc
