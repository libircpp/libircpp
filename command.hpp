
//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_CLIENT_COMMAND_HPP
#define IRC_CLIENT_COMMAND_HPP

#include <type_traits>

namespace irc {

enum class command;
//the underlying type the compiler will select for command
using raw_command_t=std::underlying_type<command>::type;

//maximum value occupied by RPLs and ERR values
constexpr raw_command_t rpl_max=999;

//convert an integral value to the enum type, checks to ensure value is in range
//assuming value is in the range of raw_command_t
command to_command(raw_command_t value);

/** 
    Reply codes and errors.
    RFC 2812: Error replies are found in the range from 400 to 599.
*/
enum class command
{
/**
    RFC 2812: "Welcome to the Internet Relay Network <nick>!<user>@<host>"
*/
    RPL_WELCOME = 001, // IEUD
/**
    RFC 2812: "Your host is <servername>, running version <ver>"
*/
    RPL_YOURHOST = 002, // IEUD
/**
    RFC 2812: "This server was created <date>"
*/
    RPL_CREATED = 003, // IEUD
/**
    RFC 2812: "<servername> <version> <available user modes> <available channel modes>"
    - The server sends Replies 001 to 004 to a user upon successful registration.
*/
    RPL_MYINFO = 004, // IEUD
/**
    RFC 2812: "Try server <server name>, port <port number>"
  - Sent by the server to a user to suggest an alternative server.
    This is often used when the connection is refused because the server is already full.
*/
    RPL_BOUNCE     = 005, // I
/*  RPL_MAP        = 005, // U
    RPL_PROTOCTL   = 005, // D
    RPL_MAPMORE    = 006, // U
    RPL_MAPEND     = 007, // U
    RPL_SNOMASK    = 008, // U
    RPL_STATMEMTOT = 009, // U
    RPL_STATMEM    = 010, // U
    RPL_YOURCOOKIE = 014, // i */
/**
    RFC 2812: "Link <version & debug level> <destination> <next server> V<protocol version>
    <link uptime in seconds> <backstream sendq> <upstream sendq>"
*/
    RPL_TRACELINK = 200, // IEUD
/**
    RFC 2812: "Try. <class> <server>"
*/
    RPL_TRACECONNECTING = 201, // IEUD
/**
    RFC 2812: "H.S. <class> <server>"
*/
    RPL_TRACEHANDSHAKE = 202, // IEUD
/**
    RFC 2812: "???? <class> [<client IP address in dot form>]"
*/
    RPL_TRACEUNKNOWN = 203, // IEUD
/**
    RFC 2812: "Oper <class> <nick>"
*/
    RPL_TRACEOPERATOR = 204, // IEUD
/**
    RFC 2812: "User <class> <nick>"
*/
    RPL_TRACEUSER = 205, // IEUD
/**
    RFC 2812: "Serv <class> <int>S <int>C <server>
    <nick!user|*!*>@<host|server> V<protocol version>"
*/
    RPL_TRACESERVER = 206, // IEUD
/**
    RFC 2812: "Service <class> <name> <type> <active type>"
*/
    RPL_TRACESERVICE = 207, // IEU
/**
    RFC 2812: "<newtype> 0 <client name>"
*/
    RPL_TRACENEWTYPE = 208, // IEUD
/**
    RFC 2812: "Class <class> <count>"
*/
    RPL_TRACECLASS = 209, // IEUD
/**
    RFC 2812: Unused.
*/
    RPL_TRACERECONNECT = 210, // I
/**
    RFC 2812: "<linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>"
    - reports statistics on a connection.
    <linkname\> identifies the particular connection,
    <sendq\> is the amount of data that is queued and waiting to be sent
    <sent messages\> the number of messages sent, and
    <sent Kbytes\> the amount of data sent, in Kbytes.
    <received messages\> and <received Kbytes\> are the equivalent of
    <sent messages\> and <sent Kbytes\> for received data, respectively.
    <time open\> indicates how long ago the connection was opened, in seconds.
*/
    RPL_STATSLINKINFO = 211, // IEUD
/**
    RFC 2812: "<command> <count> <byte count> <remote count>"
    - reports statistics on commands usage.
*/
    RPL_STATSCOMMANDS = 212, // IEUD
/**
    RFC 1459: "C <host> * <name> <port> <class>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSCLINE = 213, // IEUD
/**
    RFC 1459: "N <host> * <name> <port> <class>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSNLINE = 214, // IEUD
/**
    RFC 1459: "I <host> * <host> <port> <class>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSILINE = 215, // IEUD
/**
    RFC 1459: "K <host> * <username> <port> <class>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSKLINE = 216, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSQLINE = 217, // IED
/*  RPL_STATSPLINE = 217, // U */
/**
    RFC 1459: "Y <class> <ping frequency> <connect frequency> <max sendq>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSYLINE = 218, // IEUD
/**
    RFC 2812: "<stats letter> :End of STATS report"
*/
    RPL_ENDOFSTATS = 219, // IEUD
/**
    RFC 2812: "<user mode string>"
    - To answer a query about a client's own mode, RPL_UMODEIS is sent back.
*/
    RPL_UMODEIS = 221, // IEUD
/*
    RPL_STATSBLINE = 222, // ED
    RPL_STATSELINE = 223, // ED
    RPL_STATSFLINE = 224, // ED
    RPL_STATSDLINE = 225, // E
    RPL_STATSZLINE = 225, // D
    RPL_STATSCOUNT = 226, // D
    RPL_STATSGLINE = 227, // D
*/
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_SERVICEINFO = 231, // IU
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_ENDOFSERVICES = 232, // IU
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_SERVICE = 233, // IU
/**
    RFC 2812: "<name> <server> <mask> <type> <hopcount> <info>"
*/
    RPL_SERVLIST = 234, // IEUD
/**
    RFC 2812: "<mask> <type> :End of service listing"
    - When listing services in reply to a SERVLIST message, a server is required
    to send the list back using the RPL_SERVLIST and RPL_SERVLISTEND messages.
    A separate RPL_SERVLIST is sent for each service.
    After the services have been listed (or if none present) a RPL_SERVLISTEND MUST be sent.
*/
    RPL_SERVLISTEND = 235, // IEUD
/**
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
/*  RPL_STATSIAUTH = 239, // I */
    RPL_STATSVLINE = 240, // I
/**
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSLLINE = 241, // IEUD
/**
    RFC 2812: ":Server Up %d days %d:%02d:%02d"
*/
    RPL_STATSUPTIME = 242, // IEUD
/**
    RFC 2812: "O <hostmask> * <name>"
    - reports the allowed hosts from where user may become IRC operators.
*/
    RPL_STATSOLINE = 243, // IEUD
/**
    RFC 1459: "H <hostmask> * <servername>"
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSHLINE = 244, // IEUD
/*  RPL_STATSSLINE = 245, // IEuD */
/**
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSPING  = 246, // I
/*  RPL_STATSTLINE = 246, // U
    RPL_STATSULINE = 246, // D */
/**
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSBLINE  = 247, // I
/*  RPL_STATSGLINE  = 247, // U (REDEFINED see above)
    RPL_STATSXLINE  = 247, // E
    PL_STATSULINENE = 248, // I
    RPL_STATSULINE  = 248, // EU (REDEFINED see above)
    RPL_STATSDEBUG  = 249, // IEUD */
/**
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_STATSDLINE = 250, // I
/*  RPL_STATSCONN  = 250, // EUD */
/**
    RFC 2812: ":There are <integer> users and <integer> services on <integer> servers"
*/
    RPL_LUSERCLIENT = 251, // IEUD
/**
    RFC 2812: "<integer> :operator(s) online"
*/
    RPL_LUSEROP = 252, // IEUD
/**
    RFC 2812: "<integer> :unknown connection(s)"
*/
    RPL_LUSERUNKNOWN = 253, // IEUD
/**
    RFC 2812: "<integer> :channels formed"
*/
    RPL_LUSERCHANNELS = 254, // IEUD
/**
    RFC 2812: ":I have <integer> clients and <integer> servers"
    - In processing an LUSERS message,
    the server sends a set of replies from RPL_LUSERCLIENT, RPL_LUSEROP,
    RPL_USERUNKNOWN, RPL_LUSERCHANNELS and RPL_LUSERME.
    When replying, a server MUST send back RPL_LUSERCLIENT and RPL_LUSERME.
    The other replies are only sent back if a non-zero count is found for them.
*/
    RPL_LUSERME = 255, // IEUD
/**
    RFC 2812: "<server> :Administrative info"
*/
    RPL_ADMINME = 256, // IEUD
/**
    RFC 2812:":<admin info>"
*/
    RPL_ADMINLOC1 = 257, // IEUD
/**
    RFC 2812: ":<admin info>"
*/
    RPL_ADMINLOC2 = 258, // IEUD
/**
    RFC 2812: ":<admin info>"
*/
    RPL_ADMINEMAIL = 259, // IEUD
/**
    RFC 2812: "File <logfile> <debug level>"
    - When replying to an ADMIN message, a server is expected to use replies
    RPL_ADMINME through to RPL_ADMINEMAIL and provide a text message with each.
    For RPL_ADMINLOC1 a description of what city, state and country
    the server is in is expected, followed by details of the institution
    (RPL_ADMINLOC2) and finally the administrative contact for the server
    (an email address here is REQUIRED) in RPL_ADMINEMAIL.
*/
    RPL_TRACELOG = 261, // IEUD
/**
    RFC 2812: "<server name> <version & debug level> :End of TRACE"
    - The RPL_TRACE* are all returned by the server in response to the TRACE message.
    How many are returned is dependent on the TRACE message and
    whether it was sent by an operator or not.
    There is no predefined order for which occurs first.
    Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and RPL_TRACEHANDSHAKE
    are all used for connections which have not been fully established
    and are either unknown, still attempting to connect
    or in the process of completing the 'server handshake'.
    RPL_TRACELINK is sent by any server which handles a TRACE message
    and has to pass it on to another server.
    The list of RPL_TRACELINKs sent in response to a TRACE command
    traversing the IRC network should reflect the actual connectivity of
    the servers themselves along that path.
    RPL_TRACENEWTYPE is to be used for any connection which
    does not fit in the other categories but is being displayed anyway.
    RPL_TRACEEND is sent to indicate the end of the list.
*/
    RPL_TRACEEND = 262, // I
/*
    RPL_ENDOFTRACE = 262, // ED
    RPL_TRACEPING  = 262, // U
*/
/**
    RFC 2812: "<command> :Please wait a while and try again."
    - When a server drops a command without processing it,
    it MUST use the reply RPL_TRYAGAIN to inform the originating client.
*/
    RPL_TRYAGAIN      = 263, // I
/*  RPL_LOAD2HI       = 263, // ED
    RPL_LOCALUSERS    = 265, // ED
    RPL_GLOBALUSERS   = 266, // ED
    RPL_SILELIST      = 271, // UD
    RPL_ENDOFSILELIST = 272, // UD
    RPL_STATSDELTA    = 274, // i
    RPL_STATSDLINE    = 275, // UD (REDEFINED see above)
    RPL_GLIST         = 280, // U
    RPL_ENDOFGLIST    = 281, // U
    RPL_HELPHDR       = 290, // D
    RPL_HELPOP        = 291, // D
    RPL_HELPTLR       = 292, // D
    RPL_HELPHLP       = 293, // D
    RPL_HELPFWD       = 294, // D
    RPL_HELPIGN       = 295, // D */
/**
    RFC 1459: Dummy reply number, not used.
    RFC 2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_NONE = 300, // IEUD
/**
    RFC 2812: "<nick> :<away message>"
*/
    RPL_AWAY = 301, // IEUD
/**
    RFC 2812: ":*1<reply> *( " " <reply> )"
  - Reply format used by USERHOST to list replies to the query list.
    The reply string is composed as follows:

    reply = nickname [ "*" ] "=" ( "+" / "-" ) hostname

    The '*' indicates whether the client has registered as an Operator.
    The '-' or '+' characters represent whether the client has set
    an AWAY message or not respectively.
*/
    RPL_USERHOST = 302, // IEUD
/**
    RFC 2812: ":*1<nick> *( " " <nick> )"
    - Reply format used by ISON to list replies to the query list.
*/
    RPL_ISON = 303, // IEUD
/*  RPL_TEXT = 304, // IEUD */
/**
    RFC 2812: ":You are no longer marked as being away"
*/
    RPL_UNAWAY = 305,     // IEUD
/**
    RFC 2812: ":You have been marked as being away"
    - These replies are used with the AWAY command (if allowed).
    RPL_AWAY is sent to any client sending a PRIVMSG to a client which is away.
    RPL_AWAY is only sent by the server to which the client is connected.
    Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the client removes
    and sets an AWAY message.
*/
    RPL_NOWAWAY      = 306, // IEUD
/*  RPL_USERIP       = 307, // U
    RPL_WHOISREGNICK = 307, // D
    RPL_WHOISADMIN   = 308, // D
    RPL_WHOISSADMIN  = 309, // D
    RPL_WHOISSVCMSG  = 310, // D */
/**
    RFC 2812: "<nick> <user> <host> * :<real name>"
*/
    RPL_WHOISUSER = 311, // IEUD
/**
    RFC 2812: "<nick> <server> :<server info>"
*/
    RPL_WHOISSERVER = 312, // IEUD
/**
    RFC 2812: "<nick> :is an IRC operator"
*/
    RPL_WHOISOPERATOR = 313, // IEUD
/**
    RFC 2812: "<nick> <user> <host> * :<real name>"
*/
    RPL_WHOWASUSER = 314, // IEUD
/**
    RFC 2812: "<name> :End of WHO list"
    - The RPL_WHOREPLY and RPL_ENDOFWHO pair are used to answer a WHO message.
    The RPL_WHOREPLY is only sent if there is an appropriate match to the WHO query.
    If there is a list of parameters supplied with a WHO message,
    a RPL_ENDOFWHO MUST be sent after processing each list item with <name\> being the item.
*/
    RPL_ENDOFWHO = 315, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_WHOISCHANOP = 316, // IED
/**
    RFC 2812: "<nick> <integer> :seconds idle"
*/
    RPL_WHOISIDLE = 317, // IEUD
/**
    RFC 2812: "<nick> :End of WHOIS list"
*/
    RPL_ENDOFWHOIS = 318, // IEUD
/**
    RFC 2812: "<nick> :*( ( "@" / "+" ) <channel\> " " )"
    - Replies 311 - 313, 317 - 319 are all replies generated in response to a WHOIS message.
    Given that there are enough parameters present, the answering server
    MUST either formulate a reply out of the above numerics
    (if the query nick is found) or return an error reply.
    The '*' in RPL_WHOISUSER is there as the literal character and not as a wild card.
    For each reply set, only RPL_WHOISCHANNELS may appear more than once
    (for long lists of channel names).
    The '@' and '+' characters next to the channel name indicate whether
    a client is a channel operator or has been granted permission
    to speak on a moderated channel.
    The RPL_ENDOFWHOIS reply is used to mark the end of processing a WHOIS message.
*/
    RPL_WHOISCHANNELS = 319, // IEUD
/**
    RFC 2812: Obsolete. Not used.
*/
    RPL_LISTSTART = 321, // IEUD
/**
    RFC 2812: "<channel> <# visible> :<topic>"
*/
    RPL_LIST = 322, // IEUD
/**
    RFC 2812: ":End of LIST"
    - Replies RPL_LIST, RPL_LISTEND mark the actual replies with data and
    end of the server's response to a LIST command.
    If there are no channels available to return, only the end reply MUST be sent.
*/
    RPL_LISTEND = 323, // IEUD
/**
    RFC 2812: "<channel> <mode> <mode params>"
*/
    RPL_CHANNELMODEIS = 324, // IEUD
/**
    RFC 2812: "<channel> <nickname>"
*/
    RPL_UNIQOPIS      = 325, // I
/*  RPL_NOCHANPASS    = 326, // i
    RPL_CHPASSUNKNOWN = 327, // i
    RPL_CREATIONTIME  = 329, // EUD */
/**
    RFC 2812: "<channel> :No topic is set"
*/
    RPL_NOTOPIC = 331,  // IEUD
/**
    RFC 2812: "<channel> :<topic>"
    - When sending a TOPIC message to determine the channel topic,
    one of two replies is sent.
    If the topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC.
*/
    RPL_TOPIC         = 332, // IEUD
/*  RPL_TOPICWHOTIME  = 333, // EUD
    RPL_LISTUSAGE     = 334, // U
    RPL_COMMANDSYNTAX = 334, // D
    RPL_CHANPASSOK    = 338, // i
    RPL_BADCHANPASS   = 339, // i */
/**
    RFC 2812: "<channel> <nick>"
    - Returned by the server to indicate that the attempted INVITE message
    was successful and is being passed onto the end client.
*/
    RPL_INVITING = 341, // IEUD
/**
    RFC 2812: "<user> :Summoning user to IRC"
    - Returned by a server answering a SUMMON message to indicate
    that it is summoning that user.
*/
    RPL_SUMMONING = 342, // IED
/**
    RFC 2812: "<channel> <invitemask>"
*/
    RPL_INVITELIST = 346, // I
/**
    RFC 2812: "<channel> :End of channel invite list"
    - When listing the 'invitations masks' for a given channel,
    a server is required to send the list back using the
    RPL_INVITELIST and RPL_ENDOFINVITELIST messages.
    A separate RPL_INVITELIST is sent for each active mask.
    After the masks have been listed (or if none present)
    a RPL_ENDOFINVITELIST MUST be sent.
*/
    RPL_ENDOFINVITELIST = 347, // I
/**
    RFC 2812: "<channel> <exceptionmask>"
*/
    RPL_EXCEPTLIST = 348, // IE
/**
    RFC 2812: "<channel> :End of channel exception list"
    - When listing the 'exception masks' for a given channel,
    a server is required to send the list back using the
    RPL_EXCEPTLIST and RPL_ENDOFEXCEPTLIST messages.
    A separate RPL_EXCEPTLIST is sent for each active mask.
    After the masks have been listed (or if none present)
    a RPL_ENDOFEXCEPTLIST MUST be sent.
*/
    RPL_ENDOFEXCEPTLIST = 349, // IE
/**
    RFC 2812: "<version>.<debuglevel> <server> :<comments>"
    - Reply by the server showing its version details.
    The <version\> is the version of the software being used
    (including any patchlevel revisions) and the <debuglevel\> is used
    to indicate if the server is running in "debug mode".

    The "comments" field may contain any comments about the version
    or further version details.
*/
    RPL_VERSION = 351, // IEUD
/**
    RFC 2812: "<channel> <user> <host> <server> <nick>
    ( "H" / "G" > ["*"] [ ( "@" / "+" ) ]
    :<hopcount\> <real name\>"
*/
    RPL_WHOREPLY = 352, // IEUD
/**
    RFC 2812: "( "=" / "*" / "@" ) <channel\> :[ "@" / "+" ] <nick\> *( " " [ "@" / "+" ] <nick\> )
    - "@" is used for secret channels, "*" for private channels,
    and "=" for others (public channels).
*/
    RPL_NAMREPLY  = 353, // IEUD
/*  RPL_WHOSPCRPL = 354, // U */
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_KILLDONE = 361, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_CLOSING = 362, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_CLOSEEND = 363, // IEUD
/**
    RFC 2812: "<mask> <server> :<hopcount> <server info>"
*/
    RPL_LINKS = 364, // IEUD
/**
    RFC 2812: "<mask> :End of LINKS list"
    - In replying to the LINKS message, a server MUST send replies back
    using the RPL_LINKS numeric and mark the end of the list
    using an RPL_ENDOFLINKS reply.
*/
    RPL_ENDOFLINKS = 365, // IEUD
/**
    RFC 2812: "<channel> :End of NAMES list"
    - To reply to a NAMES message, a reply pair consisting
    of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the server back to the client.
    If there is no channel found as in the query, then only RPL_ENDOFNAMES is returned.
    The exception to this is when a NAMES message is sent with no parameters
    and all visible channels and contents are sent back
    in a series of RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark the end.
*/
    RPL_ENDOFNAMES = 366, // IEUD
/**
    RFC 2812: "<channel> <banmask>"
*/
    RPL_BANLIST = 367, // IEUD
/**
    RFC 2812: "<channel> :End of channel ban list"
    - When listing the active 'bans' for a given channel,
    a server is required to send the list back using the
    RPL_BANLIST and RPL_ENDOFBANLIST messages.
    A separate RPL_BANLIST is sent for each active banmask.
    After the banmasks have been listed (or if none present)
    a RPL_ENDOFBANLIST MUST be sent.
*/
    RPL_ENDOFBANLIST = 368, // IEUD
/**
    RFC 2812: "<nick> :End of WHOWAS"
    - When replying to a WHOWAS message, a server MUST use
    the replies RPL_WHOWASUSER, RPL_WHOISSERVER or ERR_WASNOSUCHNICK
    for each nickname in the presented list.
    At the end of all reply batches, there MUST be RPL_ENDOFWHOWAS
    (even if there was only one reply and it was an error).
*/
    RPL_ENDOFWHOWAS = 369, // IEUD
/**
    RFC 2812: ":<string>"
*/
    RPL_INFO = 371, // IEUD
/**
    RFC 2812: ":- <text>"
*/
    RPL_MOTD = 372, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_INFOSTART = 373, // IEUD
/**
    RFC 2812: ":End of INFO list"
    - A server responding to an INFO message is required to send all its 'info'
    in a series of RPL_INFO messages with a RPL_ENDOFINFO reply to indicate
    the end of the replies.
*/
    RPL_ENDOFINFO = 374, // IEUD
/**
    RFC 2812: ":- <server> Message of the day - "
*/
    RPL_MOTDSTART = 375, // IEUD
/**
    RFC 2812: ":End of MOTD command"
    - When responding to the MOTD message and the MOTD file is found,
    the file is displayed line by line, with each line no longer than 80 characters,
    using RPL_MOTD format replies.
    These MUST be surrounded by a RPL_MOTDSTART (before the RPL_MOTDs) and an
    RPL_ENDOFMOTD (after).
*/
    RPL_ENDOFMOTD = 376, // IEUD
/**
    RFC 2812: ":You are now an IRC operator"
    - RPL_YOUREOPER is sent back to a client which has just successfully
    issued an OPER message and gained operator status.
*/
    RPL_YOUREOPER = 381, // IEUD
/**
    RFC 2812: "<config file> :Rehashing"
    - If the REHASH option is used and an operator sends a REHASH message,
    an RPL_REHASHING is sent back to the operator.
*/
    RPL_REHASHING = 382, // IEUD
/**
    RFC 2812: "You are service <servicename>"
    - Sent by the server to a service upon successful registration.
*/
    RPL_YOURESERVICE = 383, // ID
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    RPL_MYPORTIS       = 384, // IEUD
/*  RPL_NOTOPERANYMORE = 385, // IEUD */
/**
    RFC 2812: "<server> :<string showing server's local time>"
    - When replying to the TIME message,
    a server MUST send the reply using the RPL_TIME format above.
    The string showing the time need only contain the correct day and time there.
    There is no further requirement for the time string.
*/
    RPL_TIME = 391, // IEUD
/**
    RFC 2812: ":UserID Terminal Host"
*/
    RPL_USERSSTART = 392, // IED
/**
    RFC 2812: ":<username> <ttyline> <hostname>"
*/
    RPL_USERS = 393, // IED
/**
    RFC 2812: ":End of users"
*/
    RPL_ENDOFUSERS = 394, // IED
/**
    RFC 2812: ":Nobody logged in"
    - If the USERS message is handled by a server,
    the replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and RPL_NOUSERS are used.
    RPL_USERSSTART MUST be sent first,
    following by either a sequence of RPL_USERS or a single RPL_NOUSER.
    Following this is RPL_ENDOFUSERS.
*/
    RPL_NOUSERS        = 395, // IED
/*  RPL_LOGON          = 600, // D
    RPL_LOGOFF         = 601, // D
    RPL_WATCHOFF       = 602, // D
    RPL_WATCHSTAT      = 603, // D
    RPL_NOWON          = 604, // D
    RPL_NOWOFF         = 605, // D
    RPL_WATCHLIST      = 606, // D
    RPL_ENDOFWATCHLIST = 607, // D
    RPL_DCCSTATUS      = 617, // D
    RPL_DCCLIST        = 618, // D
    RPL_ENDOFDCCLIST   = 619, // D
    RPL_DCCINFO        = 620, // D */
/**
    RFC 2812: "<nickname> :No such nick/channel"
    - Used to indicate the nickname parameter supplied to a command is currently unused.
*/
    ERR_NOSUCHNICK = 401, // IEUD
/**
    RFC 2812: "<server name> :No such server"
    - Used to indicate the server name given currently doesn't exist.
*/
    ERR_NOSUCHSERVER = 402, // IEUD
/**
    RFC 2812: "<channel name> :No such channel"
    - Used to indicate the given channel name is invalid.
*/
    ERR_NOSUCHCHANNEL = 403, // IEUD
/**
    RFC 2812: "<channel name> :Cannot send to channel"
    - Sent to a user who is either (a) not on a channel which is mode +n or (b)
    not a chanop (or mode +v) on a channel which has mode +m set or where
    the user is banned and is trying to send a PRIVMSG message to that channel.
*/
    ERR_CANNOTSENDTOCHAN = 404, // IEUD
/**
    RFC 2812: "<channel name> :You have joined too many channels"
    - Sent to a user when they have joined the maximum number of
    allowed channels and they try to join another channel.
*/
    ERR_TOOMANYCHANNELS = 405, // IEUD
/**
    RFC 2812: "<nickname> :There was no such nickname"
    - Returned by WHOWAS to indicate there is no history information for that nickname.
*/
    ERR_WASNOSUCHNICK = 406, // IEUD
/**
    RFC 2812: "<target> :<error code> recipients. <abort message>"
    - Returned to a client which is attempting to send a PRIVMSG/NOTICE using the
    user\@host destination format and for a user\@host which has several occurrences.
    - Returned to a client which trying to send a PRIVMSG/NOTICE to too many recipients.
    - Returned to a client which is attempting to JOIN a safe channel using the
    shortname when there are more than one such channel.
*/
    ERR_TOOMANYTARGETS = 407, // IEUD
/**
    RFC 2812: "<service name> :No such service"
    - Returned to a client which is attempting to send a SQUERY
    to a service which does not exist.
*/
    ERR_NOSUCHSERVICE  = 408, // I
/*  ERR_NOCOLORSONCHAN = 408, // D */
/**
    RFC 2812: ":No origin specified"
    - PING or PONG message missing the originator parameter.
*/
    ERR_NOORIGIN = 409, // IEUD
/**
    RFC 2812: ":No recipient given (<command>)"
*/
    ERR_NORECIPIENT = 411, // IEUD
/**
    RFC 2812: ":No text to send"
*/
    ERR_NOTEXTTOSEND = 412, // IEUD
/**
    RFC 2812: "<mask> :No toplevel domain specified"
*/
    ERR_NOTOPLEVEL = 413, // IEUD
/**
    RFC 2812: "<mask> :Wildcard in toplevel domain"
*/
    ERR_WILDTOPLEVEL = 414, // IEUD
/**
    RFC 2812: "<mask> :Bad Server/host mask"
    - 412 - 415 are returned by PRIVMSG to indicate that
    the message wasn't delivered for some reason.
    ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that are returned when
    an invalid use of "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
*/
    ERR_BADMASK        = 415, // I
/*  ERR_TOOMANYMATCHES = 416, // I
    ERR_QUERYTOOLONG   = 416, // U */
/**
    RFC 2812: "<command> :Unknown command"
    - Returned to a registered client to indicate that
    the command sent is unknown by the server.
*/
    ERR_UNKNOWNCOMMAND = 421, // IEUD
/**
    RFC 2812: ":MOTD File is missing"
    - Server's MOTD file could not be opened by the server.
*/
    ERR_NOMOTD = 422, // IEUD
/**
    RFC 2812: "<server> :No administrative info available"
    - Returned by a server in response to an ADMIN message when there is
    an error in finding the appropriate information.
*/
    ERR_NOADMININFO = 423, // IEUD
/**
    RFC 2812: ":File error doing <file op> on <file>"
    - Generic error message used to report a failed file operation
    during the processing of a message.
*/
    ERR_FILEERROR   = 424, // IED
/*  ERR_TOOMANYAWAY = 429, // D */
/**
    RFC 2812: ":No nickname given"
    - Returned when a nickname parameter expected for a command and isn't found.
*/
    ERR_NONICKNAMEGIVEN = 431, // IEUD
/**
    RFC 2812: "<nick> :Erroneous nickname"
    - Returned after receiving a NICK message which contains characters which
    do not fall in the defined set.
    See <a href="https://tools.ietf.org/html/rfc2812#section-2.3.1" target="_blank">section 2.3.1</a>
    for details on valid nicknames.
*/
    ERR_ERRONEUSNICKNAME = 432, // IEUD
/**
    RFC 2812: "<nick> :Nickname is already in use"
    - Returned when a NICK message is processed that results in an attempt
    to change to a currently existing nickname.
*/
    ERR_NICKNAMEINUSE    = 433, // IEUD
/*  ERR_SERVICENAMEINUSE = 434, // ID
    ERR_SERVICECONFUSED  = 435, // I
    ERR_BANONCHAN        = 435, // D */
/**
    RFC 2812: "<nick> :Nickname collision KILL from <user>@<host>"
    - Returned by a server to a client when it detects a nickname collision
    (registered of a NICK that already exists by another server).
*/
    ERR_NICKCOLLISION = 436, // IEUD
/**
    RFC 2812: "<nick/channel> :Nick/channel is temporarily unavailable"
    - Returned by a server to a user trying to join a channel
    currently blocked by the channel delay mechanism.
    - Returned by a server to a user trying to change nickname
    when the desired nickname is blocked by the nick delay mechanism.
*/
    ERR_UNAVAILRESOURCE = 437, // IE
/*  ERR_BANNICKCHANGE   = 437, // UD
    ERR_DEAD            = 438, // I
    ERR_NICKTOOFAST     = 438, // U
    ERR_NCHANGETOOFAST  = 438, // D
    ERR_TARGETTOOFAST   = 439, // UD
    ERR_SERVICESDOWN    = 440, // D */
/**
    RFC 2812: "<nick> <channel> :They aren't on that channel"
    - Returned by the server to indicate that the target
    user of the command is not on the given channel.
*/
    ERR_USERNOTINCHANNEL = 441, // IEUD
/**
    RFC 2812: "<channel> :You're not on that channel"
    - Returned by the server whenever a client tries to perform a channel
    affecting command for which the client isn't a member.
*/
    ERR_NOTONCHANNEL = 442, // IEUD
/**
    RFC 2812: "<user> <channel> :is already on channel"
    - Returned when a client tries to invite a user to a channel they are already on.
*/
    ERR_USERONCHANNEL = 443, // IEUD
/**
    RFC 2812: "<user> :User not logged in"
    - Returned by the summon after a SUMMON command for a user was
    unable to be performed since they were not logged in.
*/
    ERR_NOLOGIN = 444, // IED
/**
    RFC 2812: ":SUMMON has been disabled"
    - Returned as a response to the SUMMON command.
    MUST be returned by any server which doesn't implement it.
*/
    ERR_SUMMONDISABLED = 445, // IED
/**
    RFC 2812: ":USERS has been disabled"
    - Returned as a response to the USERS command.
    MUST be returned by any server which does not implement it.
*/
    ERR_USERSDISABLED = 446, // IED
/**
    RFC 2812: ":You have not registered"
    - Returned by the server to indicate that the client
    MUST be registered before the server will allow it to be parsed in detail.
*/
    ERR_NOTREGISTERED = 451, // IEUD
/*  ERR_IDCOLLISION   = 452, // i 
    ERR_NICKLOST      = 453, // i
    ERR_HOSTILENAME   = 455, // D */
/**
    RFC 2812: "<command> :Not enough parameters"
    - Returned by the server by numerous commands to
    indicate to the client that it didn't supply enough parameters.
*/
    ERR_NEEDMOREPARAMS = 461,  // IEUD
/**
    RFC 2812: ":Unauthorized command (already registered)"
    - Returned by the server to any link which tries to change part of the
    registered details (such as password or user details from second USER message).
*/
    ERR_ALREADYREGISTRED = 462, // IEUD
/**
    RFC 2812: ":Your host isn't among the privileged"
    - Returned to a client which attempts to register with a server which does not
    been setup to allow connections from the host the attempted connection is tried.
*/
    ERR_NOPERMFORHOST = 463, // IEUD
/**
    RFC 2812: ":Password incorrect"
    - Returned to indicate a failed attempt at registering a connection for
    which a password was required and was either not given or incorrect.
*/
    ERR_PASSWDMISMATCH = 464, // IEUD
/**
    RFC 2812: ":You are banned from this server"
    - Returned after an attempt to connect and register yourself with a server
    which has been setup to explicitly deny connections to you.
*/
    ERR_YOUREBANNEDCREEP = 465, // IEUD
/**
    RFC 2812: - Sent by a server to a user to inform that
    access to the server will soon be denied.
*/
    ERR_YOUWILLBEBANNED = 466, // IEUD
/**
    RFC 2812: "<channel> :Channel key already set"
*/
    ERR_KEYSET               = 467, // IEUD
/*  ERR_INVALIDUSERNAME      = 468, // U
    ERR_ONLYSERVERSCANCHANGE = 468, // D */
/**
    RFC 2812: "<channel> :Cannot join channel (+l)"
*/
    ERR_CHANNELISFULL = 471, // IEUD
/**
    RFC 2812: "<char> :is unknown mode char to me for <channel>"
*/
    ERR_UNKNOWNMODE = 472, // IEUD
/**
    RFC 2812: "<channel> :Cannot join channel (+i)"
*/
    ERR_INVITEONLYCHAN = 473, // IEUD
/**
    RFC 2812: "<channel> :Cannot join channel (+b)"
*/
    ERR_BANNEDFROMCHAN = 474, // IEUD
/**
    RFC 2812: "<channel> :Cannot join channel (+k)"
*/
    ERR_BADCHANNELKEY = 475, // IEUD
/**
    RFC 2812: "<channel> :Bad Channel Mask"
*/
    ERR_BADCHANMASK = 476, // IEUD
/**
    RFC 2812: "<channel> :Channel doesn't support modes"
*/
    ERR_NOCHANMODES    = 477, // I
/*  ERR_NEEDREGGEDNICK = 477, // D
    ERR_MODELESS       = 477, // E */
/**
    RFC 2812: "<channel> <char> :Channel list is full"
*/
    ERR_BANLISTFULL = 478, // IEUD
/*  ERR_BADCHANNAME = 479, // ED */
/**
    RFC 2812: ":Permission Denied- You're not an IRC operator"
    - Any command requiring operator privileges to operate
    MUST return this error to indicate the attempt was unsuccessful.
*/
    ERR_NOPRIVILEGES = 481, // IEUD
/**
    RFC 2812: "<channel> :You're not channel operator"
    - Any command requiring 'chanop' privileges (such as MODE messages)
    MUST return this error if the client making the attempt
    is not a chanop on the specified channel.
*/
    ERR_CHANOPRIVSNEEDED = 482, // IEUD
/**
    RFC 2812: ":You can't kill a server!"
    - Any attempts to use the KILL command on a server are to be refused and
    this error returned directly to the client.
*/
    ERR_CANTKILLSERVER = 483, // EUD
/**
    RFC 2812: ":Your connection is restricted!"
    - Sent by the server to a user upon connection to indicate
    the restricted nature of the connection (user mode "+r").
*/
    ERR_RESTRICTED    = 484,
/*  ERR_DESYNC        = 484, // ED
    ERR_ISCHANSERVICE = 484, // U */
/**
    RFC 2812: ":You're not the original channel operator"
    - Any MODE requiring "channel creator" privileges MUST return this error if
    the client making the attempt is not a chanop on the specified channel.
*/
    ERR_UNIQOPPRIVSNEEDED = 485, // I
/*  ERR_CHANTOORECENT     = 487, // i
    ERR_TSLESSCHAN        = 488, // i
    ERR_VOICENEEDED       = 489, // U */
/**
    RFC 2812: ":No O-lines for your host"
    - If a client sends an OPER message and the server has not been configured
    to allow connections from the client's host as an operator,
    this error MUST be returned.
*/
    ERR_NOOPERHOST = 491, // IEUD
/**
    RFC 1459,2812: Unused / reserved / non-generic 'feature'.
*/
    ERR_NOSERVICEHOST = 492, // I
/**
    RFC 2812: ":Unknown MODE flag"
    - Returned by the server to indicate that a MODE message was sent with
    a nickname parameter and that the a mode flag sent was not recognized.
*/
    ERR_UMODEUNKNOWNFLAG = 501, // IEUD
/**
    RFC 2812: ":Cannot change mode for other users"
    - Error sent to any user trying to view or change the
    user mode for a user other than themselves.
*/
    ERR_USERSDONTMATCH = 502, // IEUD
/*  ERR_GHOSTEDCLIENT  = 503, // ED
    ERR_LAST_ERR_MSG   = 504, // ED
    ERR_SILELISTFULL   = 511, // UD
    ERR_NOSUCHGLINE    = 512, // U
    ERR_TOOMANYWATCH   = 512, // D
    ERR_BADPING        = 513, // U
    ERR_TOOMANYDCC     = 514, // D
    ERR_LISTSYNTAX     = 521, // D
    ERR_WHOSYNTAX,     = 522, // D
    ERR_WHOLIMEXCEED   = 523, // D
    ERR_NUMERIC_ERR    = 999  // D */

    error = rpl_max + 1,
    join,
    kick,
    mode,
    nick,
    notice,
    part,
    ping,
    pong,
    privmsg,
    quit,
    topic
};

} // namespace irc

#endif //IRC_CLIENT_COMMAND_HPP
