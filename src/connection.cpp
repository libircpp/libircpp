
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "connection.hpp"

#include "util.hpp"

#include <boost/asio/connect.hpp>
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
,	socket_  { io_service                          }
,	resolver_{ io_service                          }
,	query_   { std::move(host), std::move(service) }
{	}

void connection::stop() {
	state_=states::stopped;
	socket_.cancel(); //stops async task
	//socket_.shutdown();
	//TODO: put close in dtor?
	socket_.close();
}

void connection::async_resolve() {
	resolver_.async_resolve(
		query_,
		std::bind(
			&connection::handle_resolve,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);
}

void connection::async_read() {
	assert(state_==states::active);
	boost::asio::async_read_until(
		socket_,
		streambuf_, 
		delim_,
		std::bind(
			&connection::handle_read,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);
}

void connection::async_write_next() {
	assert(!write_buffer_.empty());
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(write_buffer_.front()),
		std::bind(
			&connection::handle_write,
			shared_from_this(),
			ph::_1, ph::_2
		)
	);		
}

void connection::async_write(std::string str) {
	bool empty=write_buffer_.empty();
	write_buffer_.push_back(std::move(str));
	if(empty) async_write_next();
}

void connection::handle_read(const boost::system::error_code& error,
			                 std::size_t bytes_transferred) {
	if(!error) {
		std::istream is { &streambuf_ };
		std::string msg;

		if(std::getline(is, msg)) { //deplete the stream
			on_read_msg(msg);
		}

		if(state_==states::active) {
			async_read();
		}
	}
	else {
		on_network_error("could not read: "+error.message());
	}
}

void connection::handle_write(const boost::system::error_code& error,
                              std::size_t bytes_transferred) {
	if(!error) {
		write_buffer_.pop_front();
		if(!write_buffer_.empty())
			async_write_next();
	}
	else {
		on_network_error("could not write: "+error.message());
	}
}

void connection::handle_resolve(const boost::system::error_code& error,
					       boost::asio::ip::tcp::resolver::iterator iterator) {
	if(!error) {
		boost::asio::async_connect(
			socket_,
			iterator,
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
		state_=states::active;
		on_connect();
	}
	else {
		on_network_error("could not connect: "+error.message());
	}
}

} //namespace irc
