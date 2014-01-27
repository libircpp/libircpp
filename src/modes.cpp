
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "modes.hpp"

#include <sstream>
#include <algorithm>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace irc {

mode_block::const_iterator mode_block::find(char sym) const {
	return std::find_if(modes_.begin(), modes_.end(),
		[&](const value_type& v) { return v.first==sym; });
}
mode_block::iterator mode_block::find(char sym) {
	return std::find_if(modes_.begin(), modes_.end(),
		[&](const value_type& v) { return v.first==sym; });
}

mode_block::const_iterator mode_block::begin() const {
	return modes_.begin();
}
mode_block::const_iterator mode_block::end() const {
	return modes_.end();
}

void mode_block::set_mode_impl(char sym, const optional_string& param) {
	auto it=find(sym);
	if(it==modes_.end()) {
		modes_.emplace_back(sym, param);
	}
	else {
		it->second=param;
	}
}

void mode_block::apply_mode_diff(const prefix& p, const mode_diff& md) {
	if(md.change==mode_change::set) {
		for(const auto& m : md.modes)
			set_mode_impl(m.first, m.second);
	}
	else {
		for(const auto& m : md.modes)
			unset_mode_impl(m.first);
	}
	on_mode_change(p, md);
}



void mode_block::unset_mode_impl(char sym) {
	auto it=find(sym);
	if(it!=modes_.end()) {
		modes_.erase(it);
	}
}

bool mode_block::is_mode_set(char sym) const {
	return modes_.end()==find(sym);
}

optional_string mode_block::try_get_mode_param(char sym) {
	auto it=find(sym);
	if(it==modes_.end()) {
		throw std::runtime_error(
			"tring to retreive parater for non existant mode");
	}
	return it->second;
}

std::ostream& operator<<(std::ostream& os, const mode_block& mb) {
	for(const auto& mode : mb) {
		os << mode.first;
		if(mode.second) os << '(' << *mode.second << ')';
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const mode_list& ml) {
	for(const auto& m : ml) {
		os << m.first;
		if(m.second) os << '(' << *m.second << ')';
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const mode_diff& md) {
	return os << ( md.change == mode_change::set ? '+' : '-' )
	          << md.modes;
}

std::string to_string(const mode_block& mb) {
	std::ostringstream oss;
	oss << mb;
	return oss.str();
}

std::string to_string(const mode_list& ml) {
	std::ostringstream oss;
	oss << ml;
	return oss.str();
}

std::string to_string(const mode_diff& md) {
	std::ostringstream oss;
	oss << md;
	return oss.str();
}

} //namespace irc

BOOST_FUSION_ADAPT_STRUCT(
	irc::mode_diff,
	(irc::mode_change, change)
	(irc::mode_list,   modes)
)

namespace irc {

namespace qi=boost::spirit::qi;

mode_diff parse_modes(const std::string& entries) {
	using iterator=std::string::const_iterator;

	qi::symbols<char, mode_change> add_remove;
	add_remove.add("+", mode_change::set)("-", mode_change::unset);
	//TODO should I really use lexeme?
	qi::rule<iterator, mode_block::value_type()> mode=
		qi::lexeme[ ~qi::char_(' ')  >> -( ' ' >> +qi::char_ )];

	qi::rule<iterator, mode_diff(), qi::space_type> modes_diff=
		add_remove >> +mode;

	mode_diff md;
	qi::phrase_parse(entries.begin(), entries.end(),
		modes_diff, qi::space, md);

	return md;
}

} //namespace irc
