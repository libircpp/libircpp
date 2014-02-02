#include "simple_connection.hpp"
#include "exception.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

namespace irc {

simple_connection::simple_connection(ba::io_service& io_service)
:	io_service_ ( io_service )
,	resolver_   { io_service }
,	socket_     { io_service }
{	}

void simple_connection::handle_connect(const boost::system::error_code& error,
                                       ba::ip::tcp::resolver::iterator iter) {
	if(!error && is_ready()) { //else let it die?

		std::ostringstream oss;
		oss << "Successfully resolved destination"
			<< " Host: " << iter->host_name()
			<< " Service: " << iter->service_name();

		initiate_read();

		on_connect(oss.str());
	}
	else {
		std::ostringstream oss;
		oss << "An error occured when trying to connect: " << error.message();
		on_error(oss.str());
	}
}

void simple_connection::initiate_read() {
	if(is_ready()) {
		ba::async_read_until(socket_, streambuf_, delim_,
			std::bind(&simple_connection::handle_read,
				shared_from_this(), ph::_1, ph::_2));
	}//TODO: report if socket closed but is ative_
}

void simple_connection::handle_read(const boost::system::error_code& error,
                                    std::size_t bytes_transferred) {
	if(error) {
		std::ostringstream oss;
		oss << "An error occured when trying to connect: " << error.message();
		on_error(oss.str());
	}
	else if(is_ready()) {
		//TODO: use the full delimiter
		std::istream is { &streambuf_ };
		std::string msg;
		if(std::getline(is, msg)) { //deplete the stream
			on_read(msg);
		}
		initiate_read();
	}
}

//this can't be done in the ctor as it needs shared_from_this
void simple_connection::start(const std::string& hostname,
                              const std::string& service) {

	auto this_sp=shared_from_this();

	ba::ip::tcp::resolver::query query(hostname, service);

	active_=true;

	resolver_.async_resolve(query,
		//certain that "this" doesn't need to be captured in addition to this_sp
		//TODO: passing "this in is a horendous hack for GCC
		[this_sp, this](const boost::system::error_code& error,
			ba::ip::tcp::resolver::iterator iter) {

			if(error) {
				std::ostringstream oss;
				oss << "An error occured when trying to connect: " 
				    << error.message();
				this_sp->on_error(oss.str());
				return;
			}
			else if(this_sp->active_) { //else let it die?
				this_sp->on_resolve("Resolution completed");

				ba::async_connect(this_sp->socket_, iter,
					std::bind(&simple_connection::handle_connect,
						this_sp, ph::_1, ph::_2));
			}
		}
	);

}

void simple_connection::initiate_write() {
	assert(!message_buffer_.empty() && "message_buffer should not be empty");

	if(is_ready()) {
		boost::asio::async_write(socket_,
			boost::asio::buffer(message_buffer_.front()),
			std::bind(&simple_connection::handle_write,
				shared_from_this(), ph::_1, ph::_2));
	} //else die?
}

void simple_connection::handle_write(const boost::system::error_code& error,
                                     std::size_t bytes_transferred) {
	if(error) {
		std::ostringstream oss;
		oss << "An error occured when trying to connect: " 
			<< error.message();
		on_error(oss.str());
	}
	else {
		message_buffer_.pop_front();
		if(!message_buffer_.empty() && is_ready())
			initiate_write();
	}
}

void simple_connection::write(std::string payload) {
	if(!is_ready()) {
		std::ostringstream oss;
		oss << "can not write to inactive connection. Content: " << payload;
		throw IRC_MAKE_EXCEPTION(oss.str());
	}
	else {
		bool go=message_buffer_.empty();
		message_buffer_.push_back(std::move(payload));
		if(go) initiate_write();
	}
}

void simple_connection::disconnect() {
	active_=false;
	boost::system::error_code error;
	socket_.shutdown(ba::ip::tcp::socket::shutdown_both, error);
	socket_.close(error);
	//not exactly sure if we could report error
}

bool simple_connection::is_ready() const {
	return active_ && socket_.is_open();
}

} //namespace irc
