#include "connection.hpp"

#include "util.hpp"

#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include <functional>
#include <iostream>
#include <string>

namespace irc {

std::shared_ptr<connection> connection::make_shared(
                                boost::asio::io_service& io_service,
                                std::string host, 
                                std::string service) {
	auto icp=std::make_shared<connection>(
		io_service, 
		std::move(host), 
		std::move(service)
	);
	icp->async_resolve();
	return icp;
}

connection::connection(boost::asio::io_service& io_service,
                       std::string host,
                       std::string service) 
:	on_resolve { }
,	socket   { io_service                          }
,	resolver { io_service                          }
,	query    { std::move(host), std::move(service) }
{	}

void connection::stop() {
	state=states::stopped;
	socket.cancel(); //stops async task
	//socket.shutdown();
	//TODO: put close in dtor?
	socket.close();
}

void connection::async_resolve() {
	resolver.async_resolve(
		query,
		std::bind(
			&connection::handle_resolve,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);
}

void connection::async_read() {
	assert(state==states::active);
	boost::asio::async_read_until(
		socket,
		streambuf, 
		delim,
		std::bind(
			&connection::handle_read,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);
}

void connection::async_write_next() {
	assert(!write_buffer.empty());
	boost::asio::async_write(
		socket,
		boost::asio::buffer(write_buffer.front()),
		std::bind(
			&connection::handle_write,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);		
}

void connection::async_write(std::string str) {
	bool empty=write_buffer.empty();
	write_buffer.push_back(std::move(str));
	if(empty) async_write_next();
}

void connection::handle_read(const boost::system::error_code& error,
			                 std::size_t bytes_transferred) {
	if(error) {
		//handle error
	}
	else {
		std::istream is { &streambuf };
		std::string msg;

		if(std::getline(is, msg)) { //deplete the stream
			on_read_msg(msg);
		}

		if(state==states::active) {
			async_read();
		}
	}
}

void connection::handle_write(const boost::system::error_code& error,
                              std::size_t bytes_transferred) {
	if(error) {
		//handle error
	}
	else {
		write_buffer.pop_front();
		if(!write_buffer.empty())
			async_write_next();
	}
}

void connection::handle_resolve(const boost::system::error_code& error,
					       boost::asio::ip::tcp::resolver::iterator iterator) {
	if(!error) {
		socket.async_connect(
			*iterator,
			std::bind(
				&connection::handle_connect,
				shared_from_this(),
				ph::_1
			)
		);
		on_resolve();
	}
	else {
		on_network_error("could not resolve: "+error.message());
	}
}

void connection::handle_connect(const boost::system::error_code& error) {
	if(!error) {
		state=states::active;
		on_connect();
	}
	else {
		on_network_error("could not resolve: "+error.message());
	}
}

} //namespace irc
