#ifndef PERSISTANT_CONNECTION
#define PERSISTANT_CONNECTION

#include "types.hpp"

#include <boost/asio/io_service.hpp>

#include <string>
#include <vector>
#include <memory>

namespace irc {

/**
 * A connection which will reconnect on fatal error
 */
class persistant_connection {
	std::string                        hostname_,
	                                   service_;
	std::shared_ptr<simple_connection> connection;

	std::vector<bsig::connection> connections;

	failure_handler();
	schedule_reconnect();
public:
	persistant_connection(ba::io_service& io_service,
		std::string hostname, std::string service);

	~persistant_connection();

	const std::string get_hostname() const;
	const std::string get_service() const;

	bool is_ready() const;

	void write(const std::string& str, std::function<void(const std::string&)>);

	template<typename F> bsig::connection connect_on_resolve(F&& f);
	template<typename F> bsig::connection connect_on_connect(F&& f);

	template<typename F> bsig::connection connect_on_disconnect(F&& f);
	template<typename F> bsig::connection connect_on_reconnect(F&& f);

	template<typename F> bsig::connection connect_on_read(F&& f);
}; //class persistant_connection

} //namespace irc

#endif //PERSISTANT_CONNECTION
