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
	/**
	 * constructor for persistant_connection
	 *
	 * @param io_service the io_service to use
	 * @param hostname to connect to
	 * @param service to connect to
	 */
	persistant_connection(ba::io_service& io_service,
		std::string hostname, std::string service);
	/**
	 * destructor for persistant_connection
	 */
	~persistant_connection();
	/**
	 * gets the hostname the connection is connected to
	 *
	 * returns the hostname
	 */
	const std::string get_hostname() const;
	/**
	 * gets the service the connection is connected to,
	 * in most cases this is the port as a string. ie "6667"
	 *
	 * returns the service
	 */
	const std::string get_service() const;
	/**
	 * The connection is ready to be written to,
	 * if this is false and you attempt to write
	 * then an exception will be thrown
	 *
	 * return whether or not the connection is ready
	 */
	bool is_ready() const;

	//void write(const std::string& str, std::function<void(const std::string&)>);
	/**
	 * writes the string to the connection
	 *
	 * @param str the message to be written
	 *
	 * @throws irc::exception is throw is connection is !is_ready()
	 */
	void write(std::string str);

	void start_read();

	void stop();

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
