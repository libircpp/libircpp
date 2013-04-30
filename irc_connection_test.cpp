#include "irc_connection.hpp"

#include <boost/asio/io_service.hpp>

#include <iostream>

int main() {
	try {
		boost::asio::io_service io_service;
		auto ic=irc_connection::make_shared(io_service, "irc.freenode.net", "6667");
		ic->connect_on_resolve([]{ std::cout << "resolved"  << std::endl; });
		ic->connect_on_connect([&]{ 
			std::cout << "connected" << std::endl; 
			ic->async_write("USER zebby123432 0 * :tutorial bot\r\n");
			ic->async_write("NICK zebby123432\r\n");
		});
		
		io_service.run();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
