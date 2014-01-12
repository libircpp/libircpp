
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include "parser.hpp"
#include "types.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory> //shared_ptr
#include <string>
#include <deque>

namespace irc {

class connection : public std::enable_shared_from_this<connection> {
	enum class states { 
		resolving, active, stopped
	};
public:
	static std::shared_ptr<connection> make_shared(
	               boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

	template<typename F> bsig::connection connect_on_resolve(F&& f);
	template<typename F> bsig::connection connect_on_connect(F&& f); 
	template<typename F> bsig::connection connect_on_read_msg(F&& f);
	template<typename F> bsig::connection connect_on_network_error(F&& f);

	connection(boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

	void stop();
	void async_read();
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

	const std::string                     delim { "\r\n" };
	states                                state { states::resolving };
//signals
	sig_v  on_resolve;
	sig_v  on_connect;
	sig_s  on_read_msg;
	sig_s  on_network_error;

//asio related
	std::deque<std::string>                  write_buffer;

	boost::asio::ip::tcp::resolver::iterator endpoints;
	boost::asio::streambuf                   streambuf;
	boost::asio::ip::tcp::socket             socket;
	boost::asio::ip::tcp::resolver           resolver;
	boost::asio::ip::tcp::resolver::query    query;
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
