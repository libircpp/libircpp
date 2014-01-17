
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "command.hpp"

#include <stdexcept>

//generated using:
// s/^\s*\(ERR_\w*\).*/\tcase static_cast<raw_command_t>(\1):/p
// s/^\s*\(RPL_\w*\).*/\tcase static_cast<raw_command_t>(\1):/p

namespace irc {

command to_command(raw_command_t value) {
	switch(value) {
	default:
		throw std::runtime_error("value not an enum value");
	case static_cast<raw_command_t>(command::RPL_WELCOME):
	case static_cast<raw_command_t>(command::RPL_YOURHOST):
	case static_cast<raw_command_t>(command::RPL_CREATED):
	case static_cast<raw_command_t>(command::RPL_MYINFO):
	case static_cast<raw_command_t>(command::RPL_BOUNCE):
	case static_cast<raw_command_t>(command::RPL_TRACELINK):
	case static_cast<raw_command_t>(command::RPL_TRACECONNECTING):
	case static_cast<raw_command_t>(command::RPL_TRACEHANDSHAKE):
	case static_cast<raw_command_t>(command::RPL_TRACEUNKNOWN):
	case static_cast<raw_command_t>(command::RPL_TRACEOPERATOR):
	case static_cast<raw_command_t>(command::RPL_TRACEUSER):
	case static_cast<raw_command_t>(command::RPL_TRACESERVER):
	case static_cast<raw_command_t>(command::RPL_TRACESERVICE):
	case static_cast<raw_command_t>(command::RPL_TRACENEWTYPE):
	case static_cast<raw_command_t>(command::RPL_TRACECLASS):
	case static_cast<raw_command_t>(command::RPL_TRACERECONNECT):
	case static_cast<raw_command_t>(command::RPL_STATSLINKINFO):
	case static_cast<raw_command_t>(command::RPL_STATSCOMMANDS):
	case static_cast<raw_command_t>(command::RPL_STATSCLINE):
	case static_cast<raw_command_t>(command::RPL_STATSNLINE):
	case static_cast<raw_command_t>(command::RPL_STATSILINE):
	case static_cast<raw_command_t>(command::RPL_STATSKLINE):
	case static_cast<raw_command_t>(command::RPL_STATSQLINE):
	case static_cast<raw_command_t>(command::RPL_STATSYLINE):
	case static_cast<raw_command_t>(command::RPL_ENDOFSTATS):
	case static_cast<raw_command_t>(command::RPL_UMODEIS):
	case static_cast<raw_command_t>(command::RPL_SERVICEINFO):
	case static_cast<raw_command_t>(command::RPL_ENDOFSERVICES):
	case static_cast<raw_command_t>(command::RPL_SERVICE):
	case static_cast<raw_command_t>(command::RPL_SERVLIST):
	case static_cast<raw_command_t>(command::RPL_SERVLISTEND):
	case static_cast<raw_command_t>(command::RPL_STATSVLINE):
	case static_cast<raw_command_t>(command::RPL_STATSLLINE):
	case static_cast<raw_command_t>(command::RPL_STATSUPTIME):
	case static_cast<raw_command_t>(command::RPL_STATSOLINE):
	case static_cast<raw_command_t>(command::RPL_STATSHLINE):
	case static_cast<raw_command_t>(command::RPL_STATSPING):
	case static_cast<raw_command_t>(command::RPL_STATSBLINE):
	case static_cast<raw_command_t>(command::RPL_STATSDLINE):
	case static_cast<raw_command_t>(command::RPL_LUSERCLIENT):
	case static_cast<raw_command_t>(command::RPL_LUSEROP):
	case static_cast<raw_command_t>(command::RPL_LUSERUNKNOWN):
	case static_cast<raw_command_t>(command::RPL_LUSERCHANNELS):
	case static_cast<raw_command_t>(command::RPL_LUSERME):
	case static_cast<raw_command_t>(command::RPL_ADMINME):
	case static_cast<raw_command_t>(command::RPL_ADMINLOC1):
	case static_cast<raw_command_t>(command::RPL_ADMINLOC2):
	case static_cast<raw_command_t>(command::RPL_ADMINEMAIL):
	case static_cast<raw_command_t>(command::RPL_TRACELOG):
	case static_cast<raw_command_t>(command::RPL_TRACEEND):
	case static_cast<raw_command_t>(command::RPL_TRYAGAIN):
	case static_cast<raw_command_t>(command::RPL_NONE):
	case static_cast<raw_command_t>(command::RPL_AWAY):
	case static_cast<raw_command_t>(command::RPL_USERHOST):
	case static_cast<raw_command_t>(command::RPL_ISON):
	case static_cast<raw_command_t>(command::RPL_UNAWAY):
	case static_cast<raw_command_t>(command::RPL_NOWAWAY):
	case static_cast<raw_command_t>(command::RPL_WHOISUSER):
	case static_cast<raw_command_t>(command::RPL_WHOISSERVER):
	case static_cast<raw_command_t>(command::RPL_WHOISOPERATOR):
	case static_cast<raw_command_t>(command::RPL_WHOWASUSER):
	case static_cast<raw_command_t>(command::RPL_ENDOFWHO):
	case static_cast<raw_command_t>(command::RPL_WHOISCHANOP):
	case static_cast<raw_command_t>(command::RPL_WHOISIDLE):
	case static_cast<raw_command_t>(command::RPL_ENDOFWHOIS):
	case static_cast<raw_command_t>(command::RPL_WHOISCHANNELS):
	case static_cast<raw_command_t>(command::RPL_LISTSTART):
	case static_cast<raw_command_t>(command::RPL_LIST):
	case static_cast<raw_command_t>(command::RPL_LISTEND):
	case static_cast<raw_command_t>(command::RPL_CHANNELMODEIS):
	case static_cast<raw_command_t>(command::RPL_UNIQOPIS):
	case static_cast<raw_command_t>(command::RPL_NOTOPIC):
	case static_cast<raw_command_t>(command::RPL_TOPIC):
	case static_cast<raw_command_t>(command::RPL_INVITING):
	case static_cast<raw_command_t>(command::RPL_SUMMONING):
	case static_cast<raw_command_t>(command::RPL_INVITELIST):
	case static_cast<raw_command_t>(command::RPL_ENDOFINVITELIST):
	case static_cast<raw_command_t>(command::RPL_EXCEPTLIST):
	case static_cast<raw_command_t>(command::RPL_ENDOFEXCEPTLIST):
	case static_cast<raw_command_t>(command::RPL_VERSION):
	case static_cast<raw_command_t>(command::RPL_WHOREPLY):
	case static_cast<raw_command_t>(command::RPL_NAMREPLY):
	case static_cast<raw_command_t>(command::RPL_KILLDONE):
	case static_cast<raw_command_t>(command::RPL_CLOSING):
	case static_cast<raw_command_t>(command::RPL_CLOSEEND):
	case static_cast<raw_command_t>(command::RPL_LINKS):
	case static_cast<raw_command_t>(command::RPL_ENDOFLINKS):
	case static_cast<raw_command_t>(command::RPL_ENDOFNAMES):
	case static_cast<raw_command_t>(command::RPL_BANLIST):
	case static_cast<raw_command_t>(command::RPL_ENDOFBANLIST):
	case static_cast<raw_command_t>(command::RPL_ENDOFWHOWAS):
	case static_cast<raw_command_t>(command::RPL_INFO):
	case static_cast<raw_command_t>(command::RPL_MOTD):
	case static_cast<raw_command_t>(command::RPL_INFOSTART):
	case static_cast<raw_command_t>(command::RPL_ENDOFINFO):
	case static_cast<raw_command_t>(command::RPL_MOTDSTART):
	case static_cast<raw_command_t>(command::RPL_ENDOFMOTD):
	case static_cast<raw_command_t>(command::RPL_YOUREOPER):
	case static_cast<raw_command_t>(command::RPL_REHASHING):
	case static_cast<raw_command_t>(command::RPL_YOURESERVICE):
	case static_cast<raw_command_t>(command::RPL_MYPORTIS):
	case static_cast<raw_command_t>(command::RPL_TIME):
	case static_cast<raw_command_t>(command::RPL_USERSSTART):
	case static_cast<raw_command_t>(command::RPL_USERS):
	case static_cast<raw_command_t>(command::RPL_ENDOFUSERS):
	case static_cast<raw_command_t>(command::RPL_NOUSERS):
	case static_cast<raw_command_t>(command::ERR_NOSUCHNICK):
	case static_cast<raw_command_t>(command::ERR_NOSUCHSERVER):
	case static_cast<raw_command_t>(command::ERR_NOSUCHCHANNEL):
	case static_cast<raw_command_t>(command::ERR_CANNOTSENDTOCHAN):
	case static_cast<raw_command_t>(command::ERR_TOOMANYCHANNELS):
	case static_cast<raw_command_t>(command::ERR_WASNOSUCHNICK):
	case static_cast<raw_command_t>(command::ERR_TOOMANYTARGETS):
	case static_cast<raw_command_t>(command::ERR_NOSUCHSERVICE):
	case static_cast<raw_command_t>(command::ERR_NOORIGIN):
	case static_cast<raw_command_t>(command::ERR_NORECIPIENT):
	case static_cast<raw_command_t>(command::ERR_NOTEXTTOSEND):
	case static_cast<raw_command_t>(command::ERR_NOTOPLEVEL):
	case static_cast<raw_command_t>(command::ERR_WILDTOPLEVEL):
	case static_cast<raw_command_t>(command::ERR_BADMASK):
	case static_cast<raw_command_t>(command::ERR_UNKNOWNCOMMAND):
	case static_cast<raw_command_t>(command::ERR_NOMOTD):
	case static_cast<raw_command_t>(command::ERR_NOADMININFO):
	case static_cast<raw_command_t>(command::ERR_FILEERROR):
	case static_cast<raw_command_t>(command::ERR_NONICKNAMEGIVEN):
	case static_cast<raw_command_t>(command::ERR_ERRONEUSNICKNAME):
	case static_cast<raw_command_t>(command::ERR_NICKNAMEINUSE):
	case static_cast<raw_command_t>(command::ERR_NICKCOLLISION):
	case static_cast<raw_command_t>(command::ERR_UNAVAILRESOURCE):
	case static_cast<raw_command_t>(command::ERR_USERNOTINCHANNEL):
	case static_cast<raw_command_t>(command::ERR_NOTONCHANNEL):
	case static_cast<raw_command_t>(command::ERR_USERONCHANNEL):
	case static_cast<raw_command_t>(command::ERR_NOLOGIN):
	case static_cast<raw_command_t>(command::ERR_SUMMONDISABLED):
	case static_cast<raw_command_t>(command::ERR_USERSDISABLED):
	case static_cast<raw_command_t>(command::ERR_NOTREGISTERED):
	case static_cast<raw_command_t>(command::ERR_NEEDMOREPARAMS):
	case static_cast<raw_command_t>(command::ERR_ALREADYREGISTRED):
	case static_cast<raw_command_t>(command::ERR_NOPERMFORHOST):
	case static_cast<raw_command_t>(command::ERR_PASSWDMISMATCH):
	case static_cast<raw_command_t>(command::ERR_YOUREBANNEDCREEP):
	case static_cast<raw_command_t>(command::ERR_YOUWILLBEBANNED):
	case static_cast<raw_command_t>(command::ERR_KEYSET):
	case static_cast<raw_command_t>(command::ERR_CHANNELISFULL):
	case static_cast<raw_command_t>(command::ERR_UNKNOWNMODE):
	case static_cast<raw_command_t>(command::ERR_INVITEONLYCHAN):
	case static_cast<raw_command_t>(command::ERR_BANNEDFROMCHAN):
	case static_cast<raw_command_t>(command::ERR_BADCHANNELKEY):
	case static_cast<raw_command_t>(command::ERR_BADCHANMASK):
	case static_cast<raw_command_t>(command::ERR_NOCHANMODES):
	case static_cast<raw_command_t>(command::ERR_BANLISTFULL):
	case static_cast<raw_command_t>(command::ERR_NOPRIVILEGES):
	case static_cast<raw_command_t>(command::ERR_CHANOPRIVSNEEDED):
	case static_cast<raw_command_t>(command::ERR_CANTKILLSERVER):
	case static_cast<raw_command_t>(command::ERR_RESTRICTED):
	case static_cast<raw_command_t>(command::ERR_UNIQOPPRIVSNEEDED):
	case static_cast<raw_command_t>(command::ERR_NOOPERHOST):
	case static_cast<raw_command_t>(command::ERR_NOSERVICEHOST):
	case static_cast<raw_command_t>(command::ERR_UMODEUNKNOWNFLAG):
	case static_cast<raw_command_t>(command::ERR_USERSDONTMATCH):
	case static_cast<raw_command_t>(command::error):
	case static_cast<raw_command_t>(command::join):
	case static_cast<raw_command_t>(command::kick):
	case static_cast<raw_command_t>(command::mode):
	case static_cast<raw_command_t>(command::nick):
	case static_cast<raw_command_t>(command::notice):
	case static_cast<raw_command_t>(command::part):
	case static_cast<raw_command_t>(command::ping):
	case static_cast<raw_command_t>(command::pong):
	case static_cast<raw_command_t>(command::privmsg):
	case static_cast<raw_command_t>(command::quit):
	case static_cast<raw_command_t>(command::topic):
		return static_cast<command>(value);
	}
}

} //namespace irc
