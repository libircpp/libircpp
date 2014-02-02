#include <persistant_connection.hpp>

#include <boost/asio/io_service.hpp>

#include <boost/test/minimal.hpp>

#include <memory>
#include <iostream>
#include <exception>

/**
 * This test is pretty simple, we simply connect to a server,
 * let it end out connection (by not replying) and then watch for it
 * to reconnect
 */
int test_main(int, char**) {
	int i=0;
	try {
		boost::asio::io_service io_service;
		irc::persistant_connection connection { io_service, "irc.freenode.net", "6667" };

		connection.connect_on_resolve(
			[&i](const std::string& msg) {
				BOOST_CHECK(i==0);
				++i;
				std::cout << "RESOLVE: " << msg << std::endl;
			}
		);

		connection.connect_on_connect(
			[&i](const std::string& msg) {
				BOOST_CHECK(i==1);
				++i;
				std::cout << "CONNECT: " << msg << std::endl;
			}
		);

		connection.connect_on_disconnect(
			[&i](const std::string& msg) {
				BOOST_CHECK(i>2);
				i=0;
				std::cout << "ERROR: " << msg << std::endl;
			}
		);

		connection.connect_on_read(
			[&i](const std::string& msg) {
				BOOST_CHECK(i >= 2);
				++i;
				std::cout << "READ: " << msg << std::endl;
			}
		);

		io_service.run();
		return 0;
	}
	catch(const std::exception& e) {
		std::cerr << "exception: " << e.what() << std::endl;
		return 1;
	}
}
