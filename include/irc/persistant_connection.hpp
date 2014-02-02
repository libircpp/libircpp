#ifndef PERSISTANT_CONNECTION
#define PERSISTANT_CONNECTION

#include "types.hpp"

#include <boost/asio/io_service.hpp>

#include <string>
#include <vector>
#include <memory>

namespace irc {

namespace ba=boost::asio;

/**
 * A connection which will reconnect on fatal error
 */
class persistant_connection {
	ba::io_service&                    io_service_;
	std::string                        hostname_,
	                                   service_;
	std::shared_ptr<simple_connection> connection_;
	//TODO: add unique_connection to util and use that
	std::vector<bsig::connection>      callbacks_;

	//perhaps these could be std::functions rather than bsigs
	sig_s on_resolve;
	sig_s on_connect;
	sig_s on_disconnect;
	sig_s on_read;

	void failure_handler(const std::string& str);
	void schedule_reconnect();
	void initiate_connection();
	void clear_callbacks();
public:
	persistant_connection(ba::io_service& io_service,
		std::string hostname, std::string service);

	~persistant_connection();

	const std::string get_hostname() const;
	const std::string get_service() const;

	bool is_ready() const;

	//void write(const std::string& str, std::function<void(const std::string&)>);
	void write(const std::string& str);

	template<typename F> bsig::connection connect_on_resolve(F&& f);
	template<typename F> bsig::connection connect_on_connect(F&& f);
	template<typename F> bsig::connection connect_on_disconnect(F&& f);
	template<typename F> bsig::connection connect_on_read(F&& f);
}; //class persistant_connection

template<typename F>
bsig::connection persistant_connection::connect_on_resolve(F&& f) {
	return on_resolve.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection persistant_connection::connect_on_connect(F&& f) {
	return on_connect.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection persistant_connection::connect_on_disconnect(F&& f) {
	return on_disconnect.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection persistant_connection::connect_on_read(F&& f) {
	return on_read.connect(std::forward<F>(f));
}





} //namespace irc

#endif //PERSISTANT_CONNECTION
