#include "prefix.hpp"

namespace irc {

prefix::prefix(optional_string nick_, 
               optional_string user_, 
               optional_string host_) 
:	nick { std::move(nick_) }
,	user { std::move(user_) }
,	host { std::move(host_) }
{	}

prefix::prefix(optional_string nick_)
:	nick { std::move(nick_) }
{	}

std::ostream& operator<<(std::ostream& os, const prefix& pfx) {
	if(pfx.nick) os << '<' << *pfx.nick << '>';
	if(pfx.nick && pfx.user) os << "!";
	if(pfx.user) os << '<' << *pfx.user << '>';
	if(pfx.host && ( pfx.nick || pfx.user )) os << "@";
	if(pfx.host) os << '<' <<  *pfx.host << '>';
	return os;
}

} //namespace irc
