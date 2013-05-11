#include "session.hpp"
#include "channel.hpp"
#include "user.hpp"

#include "connection.hpp"

#include "util.hpp"

#include <iostream>

/*
** Main program state 
*/
class program {
public:
	program(boost::asio::io_service& io_service) {
		auto ic=irc::connection::make_shared(io_service, "irc.freenode.net", "6667");

		ic->connect_on_resolve([]{ std::cout << "resolved"  << std::endl; });

		ic->connect_on_connect(
			std::bind(
				&program::handle_connect,
				this,
				ic
			)
		);
	}

	void handle_connect(std::shared_ptr<irc::connection> connection) {
		assert(connection);
		sessions.push_back(
			util::make_unique<irc::session>(
				std::move(connection), 
				"test156", "test156"
			)
		);

		sessions.back()->async_join("#linux");
		sessions.back()->async_join("#bown_fox");

		sessions.back()->connect_on_motd(
			[](const std::string& motd) {
				std::cout << "MOTD\n" << motd << std::endl;
			}
		);

		sessions.back()->connect_on_join_channel(
			[](irc::channel& chan) {
				std::cout << "JOINED" << chan.get_name() << std::endl;

				chan.connect_on_user_join(
					[](const irc::channel& ch, irc::user& usr) {
						std::cout << usr.get_nick() << " at " 
						          << usr.get_prefix() << " has joined: " 
								  << ch.get_name() << std::endl;

						usr.connect_on_channel_message(
							[](const irc::channel& ch, 
							   const irc::user& usr,
							   const std::string& msg) {
								std::cout << usr.get_nick() << " -> " 
								          << ch.get_name() << " : "
										  << msg << std::endl;
							}
							
						);

					}
				);


				chan.connect_on_user_part(
					[](const irc::channel& ch, 
					   const irc::user& usr,
					   const irc::optional_string& msg) {
						std::cout << usr.get_nick() << " has left from: " 
						          << ch.get_name();
						if(msg) std::cout << " reason: " << *msg;
						std::cout << std::endl;
					}
				);
				/*
				chan.connect_on_message(
					[&](const std::string& nick, const std::string& msg) {
						if(nick=="|pipe|") {
							chan.async_send_message("hi pipe");
						}
						std::cout << nick << " said " << msg << std::endl;
					}
				);
				*/
			}
		);
			
	}
private:
	std::vector<std::unique_ptr<irc::session>> sessions;
};


int main() {
	boost::asio::io_service io_service;
	program prog { io_service };
	io_service.run();
}
