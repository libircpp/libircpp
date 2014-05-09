//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_CTCP_HPP
#define IRC_CTCP_HPP

#include <string>

namespace irc {
/**
    CTCP namespace.
    The Client-To-Client Protocol is meant to be used as a way to
    -#  in general send structured data
        (such as graphics, voice and different font information)
        between users clients, and in a more specific case:
    -#  place a query to a users client and getting an answer.
*/
namespace ctcp {

bool is_ctcp_message(const std::string& content);

/** CTCP commands. */
enum class command
{
    none = 0,           /**< Not a CTCP request / reply. **/
/**
    Used to simulate "role playing" on IRC.
    An action message looks like the following:

        \001ACTION barfs on the floor.\001

    Some IRC client could display it as:

    @b * <nickname\> barfs on the floor.
*/
    action,
/**
    Dynamic master index of what a client knows.
    This is for client developers use to make it easier to show other
    client hackers what a certain client knows when it comes to CTCP. The
    replies should be fairly verbose explaining what CTCP commands are
    understood, what arguments are expected of what type, and what replies
    might be expected from the client.

    The query is the word CLIENTINFO in a "privmsg" optionally followed by
    a colon and one or more specifying words delimited by spaces, where
    the word CLIENTINFO by itself,

        \001CLIENTINFO\001

    should be replied to by giving a list of known tags (see above in
    section TAGGED DATA). This is only intended to be read by humans.

    With one argument, the reply should be a description of how to use
    that tag. With two arguments, a description of how to use that
    tag's subcommand. And so on.
*/
    clientinfo,
/**
    Negotiates file transfers and direct tcp chat connections between clients.
    DCC stands for something like "Direct Client Connection".
    CTCP DCC extended data messages are used to negotiate file transfers between
    clients and to negotiate chat connections over tcp connections between
    two clients, with no IRC server involved. Connections between clients
    involve protocols other than the usual IRC protocol.
*/
    dcc,
/**
    Used when an error needs to be replied with.
    This is used as a reply whenever an unknown query is seen. Also, when
    used as a query, the reply should echo back the text in the query,
    together with an indication that no error has happened. Should the
    query form be used, it is

        \001ERRMSG #\001

    where # is a string containing any character, with the reply

        \001ERRMSG # :#\001

    where the first # is the same string as in the query and the second #
    a short text notifying the user that no error has occurred.

    A normal ERRMSG reply which is sent when a corrupted query or some
    corrupted extended data is received, looks like

        \001ERRMSG # :#\001

    where the first # is the the failed query or corrupted extended data
    and the second # a text explaining what the problem is, like "unknown
    query" or "failed decrypting text".
*/
    errmsg,
/**
    Returns the user's full name, and idle time.
    This is used to get a user's real name, and perhaps also the idle time
    of the user (this usage has been obsoleted by enhancements to the IRC
    protocol. The request is in a "privmsg" and looks like

        \001FINGER\001

    while the reply is in a "notice" and looks like

        \001FINGER :#\001

    where the # denotes contains information about the users real name,
    login name at clientmachine and idle time and is of type X-N-AS.
*/
    finger,
/**
    Ping is used to measure the time delay between clients on the IRC network.
    A ping query is encoded in a privmsg, and has the form:

        \001PING timestamp\001

    where `timestamp' is the current time encoded in any form the querying
    client finds convienent. The replying client sends back an identical
    message inside a notice:

        \001PING timestamp\001

    The querying client can then subtract the recieved timestamp from the
    current time to obtain the delay between clients over the IRC network.
*/
    ping,
/**
    Used to send encrypted messages between clients.
    SED probably stands for something like "Simple Encryption D???".
    It is used by clients to exchange encrypted messages between clients.
    A message encoded by SED probably looks something like:

        \001SED encrypted-text-goes-here\001
*/
    sed,
/**
    Where to obtain a copy of a client.
    This is used to get information about where to get a copy of the client.
    The request in a "privmsg" is simply

        \001SOURCE\001

    and the reply is zero or more CTCP replies of the form

        \001SOURCE #:#:#\001

    where the first # is the name of an Internet host where the client can
    be gotten from with anonymous FTP the second # a directory names, and
    the third # a space separated list of files to be gotten from that
    directory.

    Using

        X-N-SPC ::= '\000' .. '\037' | '\041' .. '\377' 

    the name of the FTP site is to be given by name like "cs.bu.edu" or
    "funic.funet.fi".

    The file name field is a directory specification optionally followed
    by one or more file names, delimited by spaces. If only a directory
    name is given, all files in that directory should be copied when
    retrieving the clients source. If some files are given, only those
    files in that directpry should be copied. Note that the spcification
    allows for all characters but space in the names, this includes
    allowing :. Examples are "pub/emacs/irc/" to get all files in
    directory pub/emacs/irc/, the client should be able to first login as
    user "ftp" and the give the command "CD pub/emacs/irc/", followed by
    the command "mget *". (It of course has to take care of binary and
    prompt mode too). Another example is "/pub/irc Kiwi.5.2.el.Z" in which
    case a "CD /pub/irc" and "get Kiwi.5.2.el.Z" is what should be done.
*/
    source,
/**
    Gets the local date and time from other clients.
    Time queries are used to determine what time it is where another
    user's client is running. This can be useful to determine if someone
    is probably awake or not, or what timezone they are in. A time query
    has the form:

        \001TIME\001

    On reciept of such a query in a privmsg, clients should reply with a
    notice of the form:

        \001TIME :human-readable-time-string\001

    For example:

        \001TIME :Thu Aug 11 22:52:51 1994 CST\001
*/
    time,
/**
    A string set by the user (never the client coder).
    This is used to transmit a string which is settable by the user (and
    never should be set by the client). The query is simply

        \001USERINFO\001

    with the reply

        \001USERINFO :#\001

    where the # is the value of the string the client's user has set.
*/
    userinfo,
/**
    The version and type of the client.
    This is used to get information about the name of the other client and
    the version of it. The request in a "privmsg" is simply

        \001VERSION\001

    and the reply

        \001VERSION #:#:#\001

    where the first # denotes the name of the client,
    the second # denotes the version of the client,
    the third # the enviroment the client is running in.

    Using

        X-N-CLN ::= '\000' .. '\071' | '\073' .. '\377' 

    the client name is a string of type X-N-CLN saying things like "Kiwi"
    or "ircII", the version saying things like "5.2" or "2.1.5c", the
    enviroment saying things like "GNU Emacs 18.57.19 under SunOS 4.1.1 on
    Sun SLC" or "Compiled with gcc -ansi under Ultrix 4.0 on VAX-11/730".
*/
    version,
    ctcp_max = version /**< CTCP command mask. **/
};

} // namespace ctcp
} // namespace irc

#endif // IRC_CTCP_HPP
