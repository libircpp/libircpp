#include "simple_connection.hpp"
#include "exception.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>

namespace irc {

simple_connection::simple_connection(ba::io_service& io_service)
:	io_service_ ( io_service )
,	resolver_   { io_service }
,	socket_     { io_service }
{	}

void simple_connection::handle_connect(const boost::system::error_code& error,
                                       ba::ip::tcp::resolver::iterator iter) {
	if(error) {
		on_error(...);
	}
	else if(is_ready()) { //else let it die?
	//	on_connect(...);
		initiate_read();
	}

}

void simple_connection::initiate_read() {

	if(is_ready()) return; //this socket is done with

	ba::async_read_until(socket_, streambuf_, delim_,
		std::bind(&simple_connection::handle_read,
			shared_from_this(), ph::_1, ph::_2));
}

void simple_connection::handle_read(const boost::system::error_code& error,
                                    std::size_t bytes_transferred) {
	if(error) {
		//on_error(...);
	}
	else if(active_) {
		initiate_read()
	}
}

//this can't be done in the ctor as it needs shared_from_this
void simple_connection::start(const std::string& hostname,
                              const std::string& service) {

	auto this_sp=shared_from_this();

	ba::ip::tcp::resolver::query query(hostname, service);

	resolver_.async_resolve(query,
		//certain that "this" doesn't need to be captured in addition to this_sp
		[this_sp](const boost::system::error_code& error,
			ba::ip::tcp::resolver::iterator iter) {

			if(error) {
				//this_sp->on_error(...);
				return;
			}
			else if(this_sp->active_) { //else let it die?
				//this_sp->on_resolve(...);

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

void simple_connection::handle_write() {
	//report message written?
	message_buffer.pop_front();
	if(!message_buffer.empty() && active_)
		initiaite_write();
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
