#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include "parser.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/signals.hpp>

#include <memory> //shared_ptr
#include <string>
#include <deque>

namespace irc {

class connection : public std::enable_shared_from_this<connection> {
	using sig_2s=boost::signal<void(std::string, std::string)>;
	using sig_v =boost::signal<void(void)>;
public:
	static std::shared_ptr<connection> make_shared(
	               boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

	template<typename F> boost::signals::connection connect_on_privmsg(F&& f) 
	{ return parser_.connect_on_privmsg(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_notice(F&& f) 
	{ return parser_.connect_on_notice(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_mode(F&& f) 
	{ return parser_.connect_on_mode(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_topic(F&& f) 
	{ return parser_.connect_on_topic(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_kick(F&& f) 
	{ return parser_.connect_on_kick(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_ping(F&& f) 
	{ return parser_.connect_on_ping(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_pong(F&& f) 
	{ return parser_.connect_on_pong(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_join(F&& f) 
	{ return parser_.connect_on_join(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_part(F&& f) 
	{ return parser_.connect_on_part(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_quit(F&& f) 
	{ return parser_.connect_on_quit(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_nick(F&& f) 
	{ return parser_.connect_on_nick(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_reply(F&& f) 
	{ return parser_.connect_on_reply(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_resolve(F&& f) 
	{ return on_resolve.connect(std::forward<F>(f)); }

	template<typename F> boost::signals::connection connect_on_connect(F&& f) 
	{ return on_connect.connect(std::forward<F>(f)); }

	connection(boost::asio::io_service& io_service,
	               std::string host, 
	               std::string service);

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
	sig_v  on_resolve;
	sig_v  on_connect;

	parser parser_;
//asio related
	const std::string                     delim {"\r\n"};
	std::deque<std::string>               write_buffer;

	boost::asio::streambuf                streambuf;
	boost::asio::ip::tcp::socket          socket;
	boost::asio::ip::tcp::resolver        resolver;
	boost::asio::ip::tcp::resolver::query query;
}; //class connection

} //namespace irc

#endif //IRC_CONNECTION_HPP
