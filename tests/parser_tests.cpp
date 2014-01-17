#define BOOST_TEST_MODULE parser_tests

#include <message.hpp>

#include <boost/test/minimal.hpp>

#include <iostream>

int test_main(int, char **) {
	

	irc::message msg;
	bool success;
	std::tie(success, msg)=irc::parse_message(":nickname 001 hello");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(!msg.prefix->user); 
	BOOST_CHECK(!msg.prefix->host); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "hello");


	std::tie(success, msg)=irc::parse_message(":nickname!username 001 hello");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(!msg.prefix->host); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "hello");


	std::tie(success, msg)=irc::parse_message(":nickname@hostname 001 hello");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(!msg.prefix->user); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "hello");

	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname 001 hello");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "hello");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname 001 hello world");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 2);
	BOOST_CHECK(msg.params[0] == "hello");
	BOOST_CHECK(msg.params[1] == "world");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname 001 :foo");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "foo");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname 001 :foo bar baz");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "foo bar baz");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname 001 hello world :foo bar baz");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::RPL_WELCOME);
	BOOST_CHECK(msg.params.size() == 3);
	BOOST_CHECK(msg.params[0] == "hello");
	BOOST_CHECK(msg.params[1] == "world");
	BOOST_CHECK(msg.params[2] == "foo bar baz");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname NICK Kilroy");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::nick);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "Kilroy");

	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname KICK #Finnish John :Speaking English");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::kick);
	BOOST_CHECK(msg.params.size() == 3);

	BOOST_CHECK(msg.params[0] == "#Finnish");
	BOOST_CHECK(msg.params[1] == "John");
	BOOST_CHECK(msg.params[2] == "Speaking English");


	std::tie(success, msg)=irc::parse_message(":nickname!username@hostname ERROR :Server *.fi already exists; ERROR message to the other server");
	BOOST_CHECK(success);
	BOOST_CHECK(msg.prefix); 
	BOOST_CHECK(msg.prefix->nick); 
	BOOST_CHECK(*msg.prefix->nick == "nickname"); 
	BOOST_CHECK(msg.prefix->user); 
	BOOST_CHECK(*msg.prefix->user == "username"); 
	BOOST_CHECK(msg.prefix->host); 
	BOOST_CHECK(*msg.prefix->host == "hostname"); 

	BOOST_CHECK(msg.command == irc::command::error);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "Server *.fi already exists; ERROR message to the other server");



	std::tie(success, msg)=irc::parse_message("KICK #Finnish John :Speaking English");
	BOOST_CHECK(success);
	BOOST_CHECK(!msg.prefix); 
	BOOST_CHECK(msg.command == irc::command::kick);
	BOOST_CHECK(msg.params.size() == 3);

	BOOST_CHECK(msg.params[0] == "#Finnish");
	BOOST_CHECK(msg.params[1] == "John");
	BOOST_CHECK(msg.params[2] == "Speaking English");


	std::tie(success, msg)=irc::parse_message("ERROR :Server *.fi already exists; ERROR message to the other server");
	BOOST_CHECK(success);
	BOOST_CHECK(!msg.prefix); 

	BOOST_CHECK(msg.command == irc::command::error);
	BOOST_CHECK(msg.params.size() == 1);
	BOOST_CHECK(msg.params[0] == "Server *.fi already exists; ERROR message to the other server");



	return 0;
}
