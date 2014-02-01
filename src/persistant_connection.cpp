#include "persistant_connection.hpp"
#include "simple_connection.hpp"

namespace irc {

persistant_connection::persistant_connection(ba::io_service& io_service,
		const std::string hostname, const std::string service)
:	io_serivce_ ( io_service          )
,	hostname_   { std::move(hostname) }
,	service_    { std::move(service)  }
{
	initiate_connection();
}

persistant_connection::~persistant_connection() {
	clear_callbacks();
	if(connection) connection->disconnect();
}

void persistant_connection::clear_callbacks() {
	for(auto& cb : callbacks)
		cb.disconnect();
	callbacks.clear();
}

void persistant_connection::failure_handler() {
	on_error(/*TODO*/);

	//At this point we have decided that our socket is done for
	//clears all the connection handlers to the basic connection
	clear_callbacks();
	connection.reset(nullptr);

	schedule_reconnect();
}


void persistant_connection::initiate_connection() {
	connection=std::make_shared<simple_connection>(
		io_service_, hostname_, service_);

	callbacks.push_back(connection->connect_on_connect(on_connect));
	callbacks.push_back(connection->connect_on_resolve(on_resolve));
	callbacks.push_back(connection->connect_on_read(on_read));
	callbacks.push_back(connection->connect_on_error(
		std::bind(&persistant_connection::failure_handler, this, ph::_1)));
	//we have to do this because connection is a shared_ptr
	connection->start();
}

void persistant_connection::schedule_reconnect() {
	//TODO: this should try again instantly on first error
	//then space out by 5seconds, 15second, 30seconds, 1min, ..., 1min, ...
	initiate_connection();
}


void persistant_connection::write(const std::string& msg) {
	if(!connection) {
		throw //...
	}
	//may also throw
	connection->write(msg);
}

} //namespace irc

