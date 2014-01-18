#include <connection.hpp>
#include <session.hpp>
#include <channel.hpp>
#include <types.hpp>


std::unique_ptr<irc::session> sess;

void on_connect(irc::shared_connection conn) {
	std::string nick="testnick123",
	            user="testuser123",
				fullname="John Smith";

	sess = std::unique_ptr<irc::session>{ 
	 	new irc::session { conn, nick, user, fullname } };

	sess->connect_on_motd([](const std::string& motd) 
		{ std::cout << "MOTD:" << motd << std::endl; });

	sess->connect_on_join_channel([](irc::channel& chan) { 
			std::cout << "Joined: " << chan.get_name() << std::endl;
			chan.connect_on_message([](const irc::channel& chan, 
				const irc::user& user, const std::string str) {
					std::cout << user.get_nick() << '@' << chan.get_name() 
							  << ": " << str << std::endl;
				}
			);
		}
	);
	sess->async_join("##news"); //or whatever
}


int main() {
	boost::asio::io_service io_service;
	std::string host="irc.freenode.net",
	            port="6667"; 

	//make a shared_ptr to a connection
	auto connection=irc::connection::make_shared(io_service, host, port);
	
	connection->connect_on_network_error(
		[](const std::string& what) { std::cerr << what << std::endl; });

	connection->connect_on_resolve(
		[]{ std::cout << "resolved" << std::endl; });

	connection->connect_on_connect(std::bind(on_connect, connection));

	io_service.run();
}

