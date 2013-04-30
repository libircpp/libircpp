#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include "irc_parser.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/signals.hpp>

#include <memory> //shared_ptr
#include <string>
#include <deque>

//callback types
namespace ph=std::placeholders;

class irc_connection : public std::enable_shared_from_this<irc_connection> {
	using sig_2s=boost::signal<void(std::string, std::string)>;
	using sig_v =boost::signal<void(void)>;
public:
	static std::shared_ptr<irc_connection> make_shared(
	               boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

	template<typename F> boost::signals::connection connect_on_priv_msg(F&& f) 
	{ return on_priv_msg.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_resolve(F f) 
	{ return on_resolve.connect(f); }
	//{ return on_resolve.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_connect(F f) 
	{ return on_connect.connect(f); }
	//{ return on_connect.connect(std::forward<F>(f)); }

	irc_connection(boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

	void async_write(std::string str);
private:
	//the move constructors could be achieved using the PIMPL idiom
	irc_connection()                                =delete;
	irc_connection(const irc_connection&)           =delete;
	irc_connection(irc_connection&&)                =delete;
	irc_connection& operator=(const irc_connection&)=delete;
	irc_connection& operator=(irc_connection&&)     =delete;

	void async_resolve();
	void async_read();
	void async_write_next();

	void handle_resolve(const boost::system::error_code& error,
	                    boost::asio::ip::tcp::resolver::iterator iterator);

	void handle_connect(const boost::system::error_code& error);


	void handle_read(const boost::system::error_code& error,
	                 std::size_t bytes_transferred);

	void handle_write(const boost::system::error_code& error,
	                  std::size_t bytes_transferred);

	void parse_message(std::string::const_iterator first, 
	                   std::string::const_iterator last);
//signals
	sig_2s on_priv_msg;
	sig_v  on_resolve;
	sig_v  on_connect;

	irc_parser parser;
//asio related
	const std::string                     delim {"\r\n"};
	std::deque<std::string>               write_buffer;

	boost::asio::streambuf                streambuf;
	boost::asio::ip::tcp::socket          socket;
	boost::asio::ip::tcp::resolver        resolver;
	boost::asio::ip::tcp::resolver::query query;
}; //class irc_connection

#endif //IRC_CONNECTION_HPP
