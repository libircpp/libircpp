#include "persistant_connection.hpp"
#include "simple_connection.hpp"
#include "exception.hpp"

namespace irc {

persistant_connection::persistant_connection(ba::io_service& io_service,
		const std::string hostname, const std::string service)
:	io_service_ ( io_service          )
,	hostname_   { std::move(hostname) }
,	service_    { std::move(service)  }
{
	initiate_connection();
}

persistant_connection::~persistant_connection() {
	clear_callbacks();
	if(connection_) connection_->disconnect();
}

void persistant_connection::clear_callbacks() {
	for(auto& cb : callbacks_)
		cb.disconnect();
	callbacks_.clear();
}

void persistant_connection::failure_handler(const std::string& str) {
	on_disconnect(str);
	//At this point we have decided that our socket is done for
	//clears all the connection handlers to the basic connection
	clear_callbacks();
	connection_.reset();

	schedule_reconnect();
}


void persistant_connection::initiate_connection() {
	connection_=std::make_shared<simple_connection>(io_service_);

	callbacks_.push_back(connection_->connect_on_connect(on_connect));
	callbacks_.push_back(connection_->connect_on_resolve(on_resolve));
	callbacks_.push_back(connection_->connect_on_read(on_read));
	callbacks_.push_back(connection_->connect_on_error(
		std::bind(&persistant_connection::failure_handler, this, ph::_1)));

	//we have to do this because connection is a shared_ptr
	connection_->start(hostname_, service_);
}

void persistant_connection::schedule_reconnect() {
	//TODO: this should try again instantly on first error
	//then space out by 5seconds, 15second, 30seconds, 1min, ..., 1min, ...
	initiate_connection();
}


void persistant_connection::write(std::string msg) {
	if(!connection_) {
		throw IRC_MAKE_EXCEPTION("Can not write to a failed socket");
	}
	//may also throw, !connection_->is_ready();
	connection_->write(std::move(msg));
}

void persistant_connection::start_read() {
	if(!connection_) {
		throw IRC_MAKE_EXCEPTION("Can not start reading on failed socket");
	}
	connection_->start_read();
}

bool persistant_connection::is_ready() const {
	return connection_ && connection_->is_ready();
}

void persistant_connection::stop() {
	if(connection_) {
		clear_callbacks();
		connection_->disconnect();
		connection_.reset();
	}
}

} //namespace irc

