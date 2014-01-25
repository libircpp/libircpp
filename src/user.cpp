
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "user.hpp"
#include "channel.hpp"

#include <utility>

namespace irc {

user_impl::user_impl(std::string nick_)
:	nick { std::move(nick_) } 
{	}

user_impl::user_impl(std::string nick_, prefix pfx_)
:	nick { std::move(nick_) } 
,	pfx  { std::move(pfx_)  }
{	}

/*
** User interface
*/
const std::string& user_impl::get_nick_impl() const { return nick; }

const prefix& user_impl::get_prefix_impl() const { return pfx; }

const mode_block& user_impl::get_modes_impl() const { return modes; }
mode_block& user_impl::get_modes_impl() { return modes; }


/*
** System interface
*/
void user_impl::set_nick(std::string nick_) {
	nick=std::move(nick_);
	on_nick_change(*this, nick);
}

void user_impl::set_prefix(prefix pfx_) {
	pfx=std::move(pfx_);
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
