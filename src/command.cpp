
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Created:     2014/01/17

#include "command.hpp"

#include <stdexcept>
#include <sstream>
#include <cassert>

//generated using:
// s/^\s*\(ERR_\w*\).*/\tcase static_cast<raw_command_t>(\1):/p
// s/^\s*\(RPL_\w*\).*/\tcase static_cast<raw_command_t>(\1):/p

namespace irc {

command to_command(raw_command_t value) {
	switch(value) {
	default: {
		std::ostringstream oss;
		oss << "Value not an enum value: " << value;
		throw std::runtime_error(oss.str());
	}
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

std::string to_string(command cmd) {
	switch(cmd) {
	case command::RPL_WELCOME: return "RPL_WELCOME";
	case command::RPL_YOURHOST: return "RPL_YOURHOST";
	case command::RPL_CREATED: return "RPL_CREATED";
	case command::RPL_MYINFO: return "RPL_MYINFO";
	case command::RPL_BOUNCE: return "RPL_BOUNCE";
	case command::RPL_TRACELINK: return "RPL_TRACELINK";
	case command::RPL_TRACECONNECTING: return "RPL_TRACECONNECTING";
	case command::RPL_TRACEHANDSHAKE: return "RPL_TRACEHANDSHAKE";
	case command::RPL_TRACEUNKNOWN: return "RPL_TRACEUNKNOWN";
	case command::RPL_TRACEOPERATOR: return "RPL_TRACEOPERATOR";
	case command::RPL_TRACEUSER: return "RPL_TRACEUSER";
	case command::RPL_TRACESERVER: return "RPL_TRACESERVER";
	case command::RPL_TRACESERVICE: return "RPL_TRACESERVICE";
	case command::RPL_TRACENEWTYPE: return "RPL_TRACENEWTYPE";
	case command::RPL_TRACECLASS: return "RPL_TRACECLASS";
	case command::RPL_TRACERECONNECT: return "RPL_TRACERECONNECT";
	case command::RPL_STATSLINKINFO: return "RPL_STATSLINKINFO";
	case command::RPL_STATSCOMMANDS: return "RPL_STATSCOMMANDS";
	case command::RPL_STATSCLINE: return "RPL_STATSCLINE";
	case command::RPL_STATSNLINE: return "RPL_STATSNLINE";
	case command::RPL_STATSILINE: return "RPL_STATSILINE";
	case command::RPL_STATSKLINE: return "RPL_STATSKLINE";
	case command::RPL_STATSQLINE: return "RPL_STATSQLINE";
	case command::RPL_STATSYLINE: return "RPL_STATSYLINE";
	case command::RPL_ENDOFSTATS: return "RPL_ENDOFSTATS";
	case command::RPL_UMODEIS: return "RPL_UMODEIS";
	case command::RPL_SERVICEINFO: return "RPL_SERVICEINFO";
	case command::RPL_ENDOFSERVICES: return "RPL_ENDOFSERVICES";
	case command::RPL_SERVICE: return "RPL_SERVICE";
	case command::RPL_SERVLIST: return "RPL_SERVLIST";
	case command::RPL_SERVLISTEND: return "RPL_SERVLISTEND";
	case command::RPL_STATSVLINE: return "RPL_STATSVLINE";
	case command::RPL_STATSLLINE: return "RPL_STATSLLINE";
	case command::RPL_STATSUPTIME: return "RPL_STATSUPTIME";
	case command::RPL_STATSOLINE: return "RPL_STATSOLINE";
	case command::RPL_STATSHLINE: return "RPL_STATSHLINE";
	case command::RPL_STATSPING: return "RPL_STATSPING";
	case command::RPL_STATSBLINE: return "RPL_STATSBLINE";
	case command::RPL_STATSDLINE: return "RPL_STATSDLINE";
	case command::RPL_LUSERCLIENT: return "RPL_LUSERCLIENT";
	case command::RPL_LUSEROP: return "RPL_LUSEROP";
	case command::RPL_LUSERUNKNOWN: return "RPL_LUSERUNKNOWN";
	case command::RPL_LUSERCHANNELS: return "RPL_LUSERCHANNELS";
	case command::RPL_LUSERME: return "RPL_LUSERME";
	case command::RPL_ADMINME: return "RPL_ADMINME";
	case command::RPL_ADMINLOC1: return "RPL_ADMINLOC1";
	case command::RPL_ADMINLOC2: return "RPL_ADMINLOC2";
	case command::RPL_ADMINEMAIL: return "RPL_ADMINEMAIL";
	case command::RPL_TRACELOG: return "RPL_TRACELOG";
	case command::RPL_TRACEEND: return "RPL_TRACEEND";
	case command::RPL_TRYAGAIN: return "RPL_TRYAGAIN";
	case command::RPL_NONE: return "RPL_NONE";
	case command::RPL_AWAY: return "RPL_AWAY";
	case command::RPL_USERHOST: return "RPL_USERHOST";
	case command::RPL_ISON: return "RPL_ISON";
	case command::RPL_UNAWAY: return "RPL_UNAWAY";
	case command::RPL_NOWAWAY: return "RPL_NOWAWAY";
	case command::RPL_WHOISUSER: return "RPL_WHOISUSER";
	case command::RPL_WHOISSERVER: return "RPL_WHOISSERVER";
	case command::RPL_WHOISOPERATOR: return "RPL_WHOISOPERATOR";
	case command::RPL_WHOWASUSER: return "RPL_WHOWASUSER";
	case command::RPL_ENDOFWHO: return "RPL_ENDOFWHO";
	case command::RPL_WHOISCHANOP: return "RPL_WHOISCHANOP";
	case command::RPL_WHOISIDLE: return "RPL_WHOISIDLE";
	case command::RPL_ENDOFWHOIS: return "RPL_ENDOFWHOIS";
	case command::RPL_WHOISCHANNELS: return "RPL_WHOISCHANNELS";
	case command::RPL_LISTSTART: return "RPL_LISTSTART";
	case command::RPL_LIST: return "RPL_LIST";
	case command::RPL_LISTEND: return "RPL_LISTEND";
	case command::RPL_CHANNELMODEIS: return "RPL_CHANNELMODEIS";
	case command::RPL_UNIQOPIS: return "RPL_UNIQOPIS";
	case command::RPL_NOTOPIC: return "RPL_NOTOPIC";
	case command::RPL_TOPIC: return "RPL_TOPIC";
	case command::RPL_INVITING: return "RPL_INVITING";
	case command::RPL_SUMMONING: return "RPL_SUMMONING";
	case command::RPL_INVITELIST: return "RPL_INVITELIST";
	case command::RPL_ENDOFINVITELIST: return "RPL_ENDOFINVITELIST";
	case command::RPL_EXCEPTLIST: return "RPL_EXCEPTLIST";
	case command::RPL_ENDOFEXCEPTLIST: return "RPL_ENDOFEXCEPTLIST";
	case command::RPL_VERSION: return "RPL_VERSION";
	case command::RPL_WHOREPLY: return "RPL_WHOREPLY";
	case command::RPL_NAMREPLY: return "RPL_NAMREPLY";
	case command::RPL_KILLDONE: return "RPL_KILLDONE";
	case command::RPL_CLOSING: return "RPL_CLOSING";
	case command::RPL_CLOSEEND: return "RPL_CLOSEEND";
	case command::RPL_LINKS: return "RPL_LINKS";
	case command::RPL_ENDOFLINKS: return "RPL_ENDOFLINKS";
	case command::RPL_ENDOFNAMES: return "RPL_ENDOFNAMES";
	case command::RPL_BANLIST: return "RPL_BANLIST";
	case command::RPL_ENDOFBANLIST: return "RPL_ENDOFBANLIST";
	case command::RPL_ENDOFWHOWAS: return "RPL_ENDOFWHOWAS";
	case command::RPL_INFO: return "RPL_INFO";
	case command::RPL_MOTD: return "RPL_MOTD";
	case command::RPL_INFOSTART: return "RPL_INFOSTART";
	case command::RPL_ENDOFINFO: return "RPL_ENDOFINFO";
	case command::RPL_MOTDSTART: return "RPL_MOTDSTART";
	case command::RPL_ENDOFMOTD: return "RPL_ENDOFMOTD";
	case command::RPL_YOUREOPER: return "RPL_YOUREOPER";
	case command::RPL_REHASHING: return "RPL_REHASHING";
	case command::RPL_YOURESERVICE: return "RPL_YOURESERVICE";
	case command::RPL_MYPORTIS: return "RPL_MYPORTIS";
	case command::RPL_TIME: return "RPL_TIME";
	case command::RPL_USERSSTART: return "RPL_USERSSTART";
	case command::RPL_USERS: return "RPL_USERS";
	case command::RPL_ENDOFUSERS: return "RPL_ENDOFUSERS";
	case command::RPL_NOUSERS: return "RPL_NOUSERS";
	case command::ERR_NOSUCHNICK: return "ERR_NOSUCHNICK";
	case command::ERR_NOSUCHSERVER: return "ERR_NOSUCHSERVER";
	case command::ERR_NOSUCHCHANNEL: return "ERR_NOSUCHCHANNEL";
	case command::ERR_CANNOTSENDTOCHAN: return "ERR_CANNOTSENDTOCHAN";
	case command::ERR_TOOMANYCHANNELS: return "ERR_TOOMANYCHANNELS";
	case command::ERR_WASNOSUCHNICK: return "ERR_WASNOSUCHNICK";
	case command::ERR_TOOMANYTARGETS: return "ERR_TOOMANYTARGETS";
	case command::ERR_NOSUCHSERVICE: return "ERR_NOSUCHSERVICE";
	case command::ERR_NOORIGIN: return "ERR_NOORIGIN";
	case command::ERR_NORECIPIENT: return "ERR_NORECIPIENT";
	case command::ERR_NOTEXTTOSEND: return "ERR_NOTEXTTOSEND";
	case command::ERR_NOTOPLEVEL: return "ERR_NOTOPLEVEL";
	case command::ERR_WILDTOPLEVEL: return "ERR_WILDTOPLEVEL";
	case command::ERR_BADMASK: return "ERR_BADMASK";
	case command::ERR_UNKNOWNCOMMAND: return "ERR_UNKNOWNCOMMAND";
	case command::ERR_NOMOTD: return "ERR_NOMOTD";
	case command::ERR_NOADMININFO: return "ERR_NOADMININFO";
	case command::ERR_FILEERROR: return "ERR_FILEERROR";
	case command::ERR_NONICKNAMEGIVEN: return "ERR_NONICKNAMEGIVEN";
	case command::ERR_ERRONEUSNICKNAME: return "ERR_ERRONEUSNICKNAME";
	case command::ERR_NICKNAMEINUSE: return "ERR_NICKNAMEINUSE";
	case command::ERR_NICKCOLLISION: return "ERR_NICKCOLLISION";
	case command::ERR_UNAVAILRESOURCE: return "ERR_UNAVAILRESOURCE";
	case command::ERR_USERNOTINCHANNEL: return "ERR_USERNOTINCHANNEL";
	case command::ERR_NOTONCHANNEL: return "ERR_NOTONCHANNEL";
	case command::ERR_USERONCHANNEL: return "ERR_USERONCHANNEL";
	case command::ERR_NOLOGIN: return "ERR_NOLOGIN";
	case command::ERR_SUMMONDISABLED: return "ERR_SUMMONDISABLED";
	case command::ERR_USERSDISABLED: return "ERR_USERSDISABLED";
	case command::ERR_NOTREGISTERED: return "ERR_NOTREGISTERED";
	case command::ERR_NEEDMOREPARAMS: return "ERR_NEEDMOREPARAMS";
	case command::ERR_ALREADYREGISTRED: return "ERR_ALREADYREGISTRED";
	case command::ERR_NOPERMFORHOST: return "ERR_NOPERMFORHOST";
	case command::ERR_PASSWDMISMATCH: return "ERR_PASSWDMISMATCH";
	case command::ERR_YOUREBANNEDCREEP: return "ERR_YOUREBANNEDCREEP";
	case command::ERR_YOUWILLBEBANNED: return "ERR_YOUWILLBEBANNED";
	case command::ERR_KEYSET: return "ERR_KEYSET";
	case command::ERR_CHANNELISFULL: return "ERR_CHANNELISFULL";
	case command::ERR_UNKNOWNMODE: return "ERR_UNKNOWNMODE";
	case command::ERR_INVITEONLYCHAN: return "ERR_INVITEONLYCHAN";
	case command::ERR_BANNEDFROMCHAN: return "ERR_BANNEDFROMCHAN";
	case command::ERR_BADCHANNELKEY: return "ERR_BADCHANNELKEY";
	case command::ERR_BADCHANMASK: return "ERR_BADCHANMASK";
	case command::ERR_NOCHANMODES: return "ERR_NOCHANMODES";
	case command::ERR_BANLISTFULL: return "ERR_BANLISTFULL";
	case command::ERR_NOPRIVILEGES: return "ERR_NOPRIVILEGES";
	case command::ERR_CHANOPRIVSNEEDED: return "ERR_CHANOPRIVSNEEDED";
	case command::ERR_CANTKILLSERVER: return "ERR_CANTKILLSERVER";
	case command::ERR_RESTRICTED: return "ERR_RESTRICTED";
	case command::ERR_UNIQOPPRIVSNEEDED: return "ERR_UNIQOPPRIVSNEEDED";
	case command::ERR_NOOPERHOST: return "ERR_NOOPERHOST";
	case command::ERR_NOSERVICEHOST: return "ERR_NOSERVICEHOST";
	case command::ERR_UMODEUNKNOWNFLAG: return "ERR_UMODEUNKNOWNFLAG";
	case command::ERR_USERSDONTMATCH: return "ERR_USERSDONTMATCH";
	case command::error: return "ERROR";
	case command::join: return "JOIN";
	case command::kick: return "KICK";
	case command::mode: return "MODE";
	case command::nick: return "NICK";
	case command::notice: return "NOTICE";
	case command::part: return "PART";
	case command::ping: return "PING";
	case command::pong: return "PONG";
	case command::privmsg: return "PRIVMSG";
	case command::quit: return "QUIT";
	case command::topic: return "TOPIC";
	default:
		assert(false); //shouldn't get here!
	}
}
} //namespace irc
