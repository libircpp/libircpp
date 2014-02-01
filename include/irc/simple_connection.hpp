#ifndef IRC_SIMPLE_CONNECTION_HPP
#define IRC_SIMPLE_CONNECTION_HPP

#include "types.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory>
#include <string>
#include <deque>

namespace irc {

namespace ba=boost::asio;

/**
 * Supports basic network IO
 *
 */
class simple_connection :
	private std::enable_shared_from_this<simple_connection> {
	using message_buffer =std::deque<std::string>;

	bool                  active_ { false };
	message_buffer        message_buffer_;
	ba::io_service&       io_service_;
	ba::ip::tcp::resolver resolver_;
	ba::ip::tcp::socket   socket_;
	ba::streambuf         streambuf_;

	const std::string     delim_ { "\r\n" };


	void initiate_read();
	void initiate_write();

	void handle_connect(const boost::system::error_code& error,
	                    ba::ip::tcp::resolver::iterator iter);

	void handle_read(const boost::system::error_code& error,
	                 std::size_t bytes_transferred);
public:
	simple_connection(ba::io_service& io_service);

	void start(const std::string& hostname, const std::string& service);
	void disconnect();
	void write(std::string str);

	bool is_ready() const;

	//This might be better as std::function?
	template<typename F> bsig::connection connect_on_resolve(F&& f);
	template<typename F> bsig::connection connect_on_connect(F&& f);
	template<typename F> bsig::connection connect_on_error(F&& f);
	template<typename F> bsig::connection connect_on_read(F&& f);
}; //class simple_session

} //namespace irc

#endif //IRC_SIMPLE_CONNECTION_HPP
