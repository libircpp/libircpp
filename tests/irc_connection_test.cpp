
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "connection.hpp"
#include "prefix.hpp"

#include <boost/asio/io_service.hpp>

#include <iostream>

int main() {
	try {
		boost::asio::io_service io_service;
		auto ic=irc::connection::make_shared(io_service, "irc.freenode.net", "6667");

		ic->connect_on_resolve([]{ std::cout << "resolved"  << std::endl; });

		/*
		ic->connect_on_notice([](const std::string& host, const std::string& notice) {
			std::cout << "notice from: " << host << " -> " << notice << std::endl;
		});
		*/

		ic->connect_on_connect([&] { 
			std::cout << "connected" << std::endl; 
			ic->async_read();
			ic->async_write("USER test156 0 * :test user\r\n");
			ic->async_write("NICK test156\r\n");
			ic->async_write("JOIN #hhjjkk\r\n");
			//ic->async_write("JOIN #jumped_over\r\n");
			//ic->async_write("JOIN #lazy_dog\r\n");
			//ic->async_write("JOIN #linux\r\n");
		});

		ic->connect_on_privmsg([&](const irc::prefix& pfx,
		                           const std::vector<std::string>& to, 
                                   const std::string& msg) {
			std::cout << "privmsg to: ";
			for(const auto& s : to) std::cout << s << " ";

			std::cout << "from: " << pfx << " -> " << msg << std::endl;
		});

		ic->connect_on_notice([&](const irc::prefix& pfx,
		                          const std::string& nick,
		                          const std::string& msg) {
			std::cout << "notice to: " << nick <<" from " << pfx << " -> " << msg << std::endl;
		});

		ic->connect_on_mode([&](const std::string& target, const std::string& mode) {
			std::cout << "mode for " << target << " -> " << mode << std::endl;
		});

		ic->connect_on_topic([&](const std::string& target, const std::string& topic) {
			std::cout << "topic for " << target << " -> " << topic << std::endl;
		});

		ic->connect_on_kick([&](const std::string& channel, 
		                        const std::string& user, 
								const irc::optional_string& comment) {
			std::cout << "user " << user << "  kick from " << channel;
			if(comment) std::cout << " reason: " << *comment;
			std::cout << std::endl;
		});

		ic->connect_on_ping([&](const irc::prefix&          pfx,
		                        const std::string&     server1,
		                        const irc::optional_string& server2) {
			std::cout << "Ping from: " << pfx << "  server1   " << server1 << "  server2? " << server2 << std::endl;

			ic->async_write("PONG zebby123432 " + server1 + "\r\n");
		});

		ic->connect_on_join([&](const irc::prefix&          pfx,
		                        const std::string&     chan) {
			std::cout << pfx << " has joined " << chan << std::endl;
		});

		ic->connect_on_part([&](const irc::prefix&          pfx,
		                        const std::string&     chan,
		                        const irc::optional_string& msg) {

			std::cout << pfx << " has parted " << chan ;
			if(msg) std::cout << "  with " << *msg;
			std::cout << std::endl;
		});

		ic->connect_on_quit([&](const irc::prefix&      pfx,
		                        const std::string& msg) {
			std::cout << pfx << " has quit with msg: " << msg << std::endl;
		});

		ic->connect_on_nick([&](const irc::prefix&      pfx,
		                        const std::string& nick) {
			std::cout << pfx << "  has set their nick to: " << nick << std::endl;
		});

		ic->connect_on_reply([&](const irc::prefix&                   pfx,
	                            int                             value,
	                            const std::vector<std::string>& params) {
			std::cout << pfx << " value: " << value << '\n';
			for(const auto& param :  params) {
				std::cout << param << std::endl;			
		}
	});

		io_service.run();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
