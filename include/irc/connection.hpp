
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include "types.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory> //shared_ptr
#include <string>
#include <deque>

namespace irc {
/**
 * IRC connection class.
 */
class connection : public std::enable_shared_from_this<connection> {
	enum class states {
		resolving, active, stopped
	};
public:
	/**
	 * Static constructor.
	 * @param io_service A reference to the ASIO io_service.
	 * @param host       An hostname to connect to.
	 * @param service    The connection port as string.
	 * @return A shared_ptr to the created connection.
	 */
	static std::shared_ptr<connection> make_shared(
	               boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);
	/**
	 * Connect to the on_resolve signal.
	 * This signal is triggered when IRC server names was resolved.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f()
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_resolve(F&& f);
	/**
	 * Connect to the on_connect signal.
	 * This signal is triggered when connected to an IRC server.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f()
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_connect(F&& f); 
	/**
	 * Connect to the on_read_msg signal.
	 * This signal is triggered when an IRC server message was read.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(const std::string& msg)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_read_msg(F&& f);
	/**
	 * Connect to the on_network_error signal.
	 * This signal is triggered when there was a connection error.
	 *
	 * @param f A callback function with the following signature:
	 * @code void f(const std::string& error_msg)
	 * @endcode
	 * @return The connection object to disconnect from the signal.
	 */
	template<typename F> bsig::connection connect_on_network_error(F&& f);
	/**
	 * Constructor.
	 * @param io_service A reference to the ASIO io_service.
	 * @param host       An hostname to connect to.
	 * @param service    The connection port as string.
	 */
	connection(boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);
	/**
	 * Disconnects from IRC server.
	 */
	void stop();
	/**
	 * Requests an asynchronous buffer read from the socket.
	 */
	void async_read();
	/**
	 * Requests an asynchronous buffer write to the socket.
	 * @param str Data to push back to the write buffer.
	 */
	void async_write(std::string str);
private:
	//the move constructors could be achieved using the PIMPL idiom
	connection()                            =delete;
	connection(const connection&)           =delete;
	connection(connection&&)                =delete;
	connection& operator=(const connection&)=delete;
	connection& operator=(connection&&)     =delete;

	void async_resolve();
	void async_write_next();
	void try_connect();

	void handle_resolve(const boost::system::error_code& error,
	                    boost::asio::ip::tcp::resolver::iterator iterator);
	void handle_connect(const boost::system::error_code& error);
	void handle_read(   const boost::system::error_code& error,
	                    std::size_t bytes_transferred);
	void handle_write(  const boost::system::error_code& error,
	                    std::size_t bytes_transferred);

	const std::string                     delim_{ "\r\n" };
	states                                state_{ states::resolving };
//signals
	sig_v  on_resolve;
	sig_v  on_connect;
	sig_s  on_read_msg;
	sig_s  on_network_error;

//asio related
	std::deque<std::string>                  write_buffer_;

	boost::asio::ip::tcp::resolver::iterator endpoints_;
	boost::asio::streambuf                   streambuf_;
	boost::asio::ip::tcp::socket             socket_;
	boost::asio::ip::tcp::resolver           resolver_;
	boost::asio::ip::tcp::resolver::query    query_;
}; //class connection


template<typename F> 
bsig::connection connection::connect_on_resolve(F&& f) {
	return on_resolve.connect(std::forward<F>(f));
}

template<typename F> 
bsig::connection connection::connect_on_connect(F&& f) {
	return on_connect.connect(std::forward<F>(f));
}

template<typename F>
bsig::connection connection::connect_on_read_msg(F&& f) {
	return on_read_msg.connect(std::forward<F>(f));
}

template<typename F>
bsig::connection connection::connect_on_network_error(F&& f) {
	return on_network_error.connect(std::forward<F>(f));
}

} //namespace irc

#endif //IRC_CONNECTION_HPP
