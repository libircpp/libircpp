
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "exception.hpp"
#include "session.hpp"
#include "persistant_connection.hpp"
#include "message.hpp"
#include "channel.hpp"
#include "prefix.hpp"
#include "user.hpp"

#include "util.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include <tuple> //tie
#include <sstream> //ostringstream

namespace irc {

bool is_operator(const std::string& s) { 
	return !s.empty() && s[0]=='@'; 
}

bool is_channel(const std::string& target) {
    return !target.empty()
	    && (target[0]=='#' || target[0]=='&' ||
		    target[0]=='+' || target[0]=='!');
}

void session::join_sequence() {
	std::ostringstream oss;
	oss << "USER " << username_ << " 0 * :";


	if(realname_.empty()) oss << "*\r\n";
	else                  oss << realname_ << "\r\n";

	connection_->write(oss.str());
	connection_->write("NICK "+nickname_+"\r\n");
}

void session::rejoin_sequence() {
	join_sequence();
}

void session::handle_connection_established() {
	active_=true;
	//TODO: rejoining channels functionality
	//rejoin_channels();
	on_connection_established();
}

void session::handle_nick_in_use() {
	auto self_it=get_or_create_user(nickname_);

	auto nn=on_new_nick(std::move(nickname_));
	if(nn) nickname_=std::move(*nn);
	else   nickname_+='_';

	self_it->second->set_nick(nickname_); //why do we keep these in sync? just use one?
	users_[nickname_]=std::move(self_it->second);
	users_.erase(self_it);

	connection_->write("NICK "+nickname_+"\r\n");
}


void session::prepare_connection() {
	assert(connection_);

	on_connect_handle.disconnect();

	connection_->connect_on_read(
		[&](const std::string& raw_msg) {
			try {
				bool success;
				message msg;
				std::tie(success, msg)=parse_message(raw_msg);
				if(success) {
					handle_reply(msg.prefix ? *msg.prefix : prefix{},
						msg.command, msg.params);
				}
				else {
					std::ostringstream oss;
					oss << "could not parse command: " << raw_msg;
					on_protocol_error(oss.str());
				}
			}
			catch(const std::exception& e) {
				//TODO: we need to be more specific here, these all irc_errors
				std::ostringstream oss;
				oss << "could not parse command: " << e.what();
				on_irc_error(oss.str());
			}
		}
	);
	connection_->start_read();
	connection_->connect_on_disconnect(
		[this](const std::string& msg) {
			active_=false;
		}
	);
	join_sequence();
}

session::session(std::unique_ptr<persistant_connection>&& conn,
                 std::string nickname,
                 std::string username,
				 std::string realname)
:	connection_ { std::move(conn)     }
,	nickname_   { std::move(nickname) }
,	username_   { std::move(username) }
,	realname_   { std::move(realname) }
{
	assert(connection_ && "connection is invalid from start");

	if(connection_->is_ready()) {
		prepare_connection();
	}
	else {
		on_connect_handle=connection_->connect_on_connect(
			std::bind(&session::prepare_connection, this));
	}
}


session::channel_iterator session::create_new_channel(const std::string& channel_name) {
	assert(channels_.count(channel_name)==0);

	channel_iterator it;
	bool             success;

	std::tie(it, success)=channels_.emplace(
		channel_name, std::make_shared<channel_impl>(*this, channel_name));

	if(!success)
		throw IRC_MAKE_EXCEPTION("Unable to insert new channel: " + channel_name);

	return it;
}

session::channel_iterator session::get_or_create_channel(const std::string& channel_name) {
	auto it=channels_.find(channel_name);

	if(it!=channels_.cend())
		return it;
	else
		return create_new_channel(channel_name);
}

session::user_iterator session::create_new_user(const std::string& name,
                                                const prefix& pfx) {
	assert(users_.count(name)==0);
	user_iterator it;
	bool          success;

	std::tie(it, success)=users_.emplace(
		name, std::make_shared<user_impl>(name, pfx));

	if(name!=get_nick()) { //or maybe user==get_self() ?
		on_new_user(*it->second);
	}

	if(!success)
		throw IRC_MAKE_EXCEPTION("Unable to insert new user: " + name);

	return it;
}

session::user_iterator session::get_or_create_user(const std::string& username) {
	assert(username.size() > 0);

	const auto& mus=is_operator(username) ? username.substr(1) : username;

	auto it=users_.find(mus);

	if(it!=users_.cend())
		return it;
	else
		return create_new_user(mus, { mus });
}

session::user_iterator session::get_or_create_user(const prefix& pfx) {
	assert(pfx.nick);
	auto it=users_.find(*pfx.nick);

	if(it!=users_.cend())
		return it;
	else
		return create_new_user(*pfx.nick, pfx);
}


/*
** HANDLERS
*/
void session::handle_privmsg(const prefix& pfx,
                             const std::string& target,
                             const std::string& content) {
	if(pfx.nick) { //nick is an optional
		auto user=get_or_create_user(pfx)->second; //TODO: by ref or move?
		assert(user);
		if(target == nickname_) { //1 to 1
			user->direct_message(content);
		}
		else { //1 to channel
			auto chan=get_or_create_channel(target)->second;
			assert(chan);

			chan->message(user, content);
			user->channel_message(chan, content);
		}
	}
}


void session::handle_notice (const prefix&      pfx,
                             const std::string& target,
                             const std::string& msg) {
	if(pfx.nick) {
		auto user=get_or_create_user(pfx)->second;
		assert(user);
		user->notice(msg);
		on_user_notice(*user, msg);
	}
	else {
		on_notice(msg);
	}
}

void session::handle_topic(const std::string& channel_name, std::string topic) {
	auto chan=get_or_create_channel(channel_name)->second;
	assert(chan);
	chan->set_topic(std::move(topic));
}

void session::handle_ping(const prefix& pfx,
                          const std::string& server1,
                          const optional_string& server2) {
	std::ostringstream oss;
	oss << "PONG " << nickname_ << " " << server1 << "\r\n";
	connection_->write(oss.str());
}

void session::handle_join(const prefix& pfx,
                          const std::string& channel_name) {
	if(pfx.nick) {
		auto chan=get_or_create_channel(channel_name)->second;
		auto user=get_or_create_user(pfx)->second;
		assert(chan);
		assert(user);
		chan->user_join(user);
		if(user->get_nick() == nickname_) { //is_me?
			on_join_channel(*chan);
		}
	}
}

void session::handle_part(const prefix& pfx,
                          const std::string& channel_name,
                          const optional_string& msg) {
	//TODO have just get
	auto chan=get_or_create_channel(channel_name)->second;
	auto user_it=get_or_create_user(pfx);
	auto user_p=user_it->second;

	assert(chan);
	assert(user_p);

	if(user_p->get_nick()==get_self().get_nick()) {
		//we havea left a channel
		auto ch_it=channels_.find(channel_name);
		//TODO: perhaps set warning if channel isn't even in list?
		if(ch_it!=channels_.end()) {
			ch_it->second->part();
		}
		channels_.erase(ch_it);
	}
	else {
		//a user has left a channel

		chan->user_part(user_p, msg);

		users_.erase(user_it);
	}
}

void session::handle_quit(const prefix& pfx,
                          const std::string& msg) {
	if(pfx.nick) {
		auto user_it=get_or_create_user(pfx); //todo: optimise
		auto user   =user_it->second;

		for(auto& channel : channels_) {
			channel.second->user_quit(user, msg);
		}
		users_.erase(user_it);
	}
	else {
		on_protocol_error(
			"QUIT message recieved for missing nick: prefix is: " + 
				to_string(pfx));
	}
}

// numeric responses from the server
void session::handle_reply(const prefix& pfx, command cmd, 
                           const std::vector<std::string>& params) {

	auto requires_n_params=[&](std::size_t n) {
		if(params.size() != n) {
			//TODO on_irc_error(...)
			return false;
		}
		return true;
	};
	auto minimum_n_params=[&](std::size_t n) {
		if(params.size() < n) {
			//TODO on_irc_error(...)
			return false;
		}
		return true;
	};

	switch(cmd) {
	//MAIN actions
	case command::error:
		break;
	case command::join: if(requires_n_params(1))
		handle_join(pfx, params[0]);
		break;
	case command::kick:
		break;
	case command::mode:
		if(minimum_n_params(2)) {
			std::string modes=params[1];
			if(params.size() == 3) modes+=" " + params[2];
			handle_mode(pfx, params[0], modes);
		}
		break;
	case command::nick:
		//TODO
		break;
	case command::notice: if(requires_n_params(2))
		handle_notice(pfx, params[0], params[1]);
		break;
	case command::part: if(minimum_n_params(1))
		handle_part(pfx, params[0], util::try_get(params, 1));
		break;
	case command::ping: if(minimum_n_params(1))
		handle_ping(pfx, params[0], util::try_get(params, 1));
		break;
	case command::pong:
		//TODO do we care?
		break;
	case command::privmsg:
		if(requires_n_params(2)) {
			handle_privmsg(pfx, params[0], params[1]);
		}
		break;
	case command::quit: if(requires_n_params(1))
		handle_quit(pfx, params[0]);
		break;
	case command::topic: if(requires_n_params(2))
		handle_topic(params[0], params[1]);
		break;


	case command::ERR_NOSUCHCHANNEL: // 403,
		on_irc_error("No such channel");
		break;

	case command::ERR_NICKNAMEINUSE: // 403,
		handle_nick_in_use();
		break;

	case command::RPL_WELCOME:
		//it would be wrong to receive this and active_ be true
		if(!active_)
			handle_connection_established();
		break;
	case command::RPL_MOTD:
	{
		std::ostringstream oss; //TODO optimise for size=1 case?
		if(!motd_.empty()) oss << '\n';
		if(params.size() > 1) {
			std::copy(params.cbegin()+1, params.cend(),
				std::ostream_iterator<std::string>(oss, " "));
		}
		motd_+=oss.str();
		break;
	}
	case command::RPL_MOTDSTART:
		if(!active_) handle_connection_established();
		motd_.clear();
		break;
	case command::RPL_ENDOFMOTD:
		on_motd(motd_);
		break;
	case command::RPL_NAMREPLY:
		if(params.size() > 3) { //

			auto chan=get_or_create_channel(params[2])->second;
			assert(chan);

			std::for_each(params.cbegin() + 3, params.cend(),
				[&](const std::string& param) {
					std::istringstream iss { param };
					std::for_each(
						std::istream_iterator<std::string> { iss }, { },
						[&](const std::string& nick) {
							auto user=get_or_create_user(nick)->second;
							assert(user);
							//note this add user, not user join
							chan->add_user(user);
						}
					);
				}
			);
		}
		break;
	case command::RPL_ENDOFNAMES:
		if(params.size() > 1) { //1st is usr name, 2nd is chan name
			auto chan=get_or_create_channel(params[1])->second;
			chan->list_users();
		}
		break;
	case command::RPL_TOPIC:
		if(params.size() > 2) {
			auto chan=get_or_create_channel(params[1])->second;
			chan->set_topic(params[2]);
		}
		break;
	default: {
			auto cmd_as_string=to_string(cmd);
			if(boost::starts_with(cmd_as_string, "ERR_")) {
				std::ostringstream oss;
				oss << cmd_as_string;
				if(!params.empty()) {
					oss << ": ";
					for(const auto& s : params) oss << s << " ";
				}
				on_irc_error(oss.str());
			}
			else if(boost::starts_with(cmd_as_string, "RPL_")) {
				//TODO: on reply, maybe we don't care, as it basically suggests
				//that what we asked was ok?
			}
		}
		break;
	}
}


void session::handle_mode(const prefix& pfx, 
                          const std::string& agent,
                          const std::string& mode) {

	mode_diff parsed_modes;
	parsed_modes=parse_modes(mode);

	if(is_channel(agent)) {
		auto chan=get_or_create_channel(agent)->second;
		assert(chan);

		chan->apply_mode_diff(pfx, std::move(parsed_modes));
	}
	else { //is user
		auto& user=get_or_create_user(agent)->second;
		assert(user);

		user->get_modes().apply_mode_diff(pfx, parsed_modes);
	}
}	

const std::string& session::get_nick() const {
	return nickname_;
}


user& session::get_self() {
	//TODO: if user doesn't exist then make?
	auto it=get_or_create_user(get_nick());

	if(it==end(users_)) {
		assert(false && "not implemented");
	}
	assert(it->second); //should be no nullptr shared_ptr in users
	return *it->second;
}


session::const_user_iterator session::begin_users() const {
	return boost::make_transform_iterator(
		begin(users_),
		second_deref{}
	);
}
session::const_user_iterator session::end_users() const {
	return boost::make_transform_iterator(
		end(users_),
		second_deref{}
	);
}

session::const_channel_iterator session::channel_begin() const {
	return boost::make_transform_iterator(
		begin(channels_),
		second_deref{}
	);

}
session::const_channel_iterator session::channel_end()   const {
	return boost::make_transform_iterator(
		end(channels_),
		second_deref{}
	);
}

/*
** async interface
*/

void session::async_part(const channel& chan, const std::string& msg) {
	std::ostringstream oss;
	oss << "PART " << chan.get_name() << " :" << msg << "\r\n";
	connection_->write(oss.str());
}
void session::async_join(const std::string& channel_name) {
	std::ostringstream oss;
	oss << "JOIN " << channel_name << "\r\n";
	connection_->write(oss.str());
}
void session::async_privmsg(const std::string& target, const std::string& msg) {
	std::ostringstream oss;
	oss << "PRIVMSG " << target << " :" << msg << "\r\n";
	connection_->write(oss.str());
}
void session::async_change_nick(const std::string& desired_nick) {
	std::ostringstream oss;
	oss << "NICK " << desired_nick << "\r\n";
	connection_->write(oss.str());
}

void session::stop() {
	//should we quit, probably too late if we are just going to stop?
	active_=false;
	if(connection_) {
		connection_->stop();
	}
}


} //namespace irc
