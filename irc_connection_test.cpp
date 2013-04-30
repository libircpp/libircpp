#include "irc_connection.hpp"

#include <boost/asio/io_service.hpp>

#include <iostream>

int main() {
	try {
		boost::asio::io_service io_service;
		auto ic=irc_connection::make_shared(io_service, "irc.freenode.net", "6667");

		ic->connect_on_resolve([]{ std::cout << "resolved"  << std::endl; });

		ic->connect_on_notice([](const std::string& host, const std::string& notice) {
			std::cout << "notice from: " << host << " -> " << notice << std::endl;
		});

		ic->connect_on_connect([&] { 
			std::cout << "connected" << std::endl; 
			ic->async_write("USER zebby123432 0 * :tutorial bot\r\n");
			ic->async_write("NICK zebby123432\r\n");
			ic->async_write("JOIN #brown_fox\r\n");
		});

		ic->connect_on_privmsg([&](const std::vector<std::string>& to, const std::string& msg) {
			std::cout << "privmsg to: ";
			for(const auto& s : to) std::cout << s << " ";
			std::cout << " -> " << msg << std::endl;
		});

		ic->connect_on_mode([&](const std::string& target, const std::string& mode) {
			std::cout << "mode for " << target << " -> " << mode << std::endl;
		});

		ic->connect_on_topic([&](const std::string& target, const std::string& topic) {
			std::cout << "topic for " << target << " -> " << topic << std::endl;
		});

		ic->connect_on_kick([&](const std::string& channel, 
		                        const std::string& user, 
								const optional_string& comment) {
			std::cout << "user " << user << "  kick from " << channel;
			if(comment) std::cout << " reason: " << *comment;
			std::cout << std::endl;
		});

		io_service.run();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
