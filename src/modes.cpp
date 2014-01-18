#include "modes.hpp"

#include <sstream>
#include <algorithm>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace irc {

mode_block::const_iterator mode_block::find(char sym) const {
	return std::find_if(modes.begin(), modes.end(), 
		[&](const value_type& v) { return v.first==sym; });
}
mode_block::iterator mode_block::find(char sym) {
	return std::find_if(modes.begin(), modes.end(), 
		[&](const value_type& v) { return v.first==sym; });
}

mode_block::const_iterator mode_block::begin() const {
	return modes.begin();
}
mode_block::const_iterator mode_block::end() const {
	return modes.end();
}



void mode_block::set_mode_impl(char sym, const optional_string& param) {
	auto it=find(sym);
	if(it==modes.end()) {
		modes.emplace_back(sym, param);			
	}
	else {
		it->second=param;
	}
}
void mode_block::set_mode(char sym, const optional_string& param) {
	set_mode_impl(sym, param);
	on_set_mode({std::make_pair(sym, param)});
}
void mode_block::set_mode(const std::vector<value_type>& params) {
	for(const auto& param : params) {
		set_mode_impl(param.first, param.second);
	}
	on_set_mode(params);
}



void mode_block::unset_mode_impl(char sym) {
	auto it=find(sym);
	if(it!=modes.end()) {
		modes.erase(it);			
	}
}
void mode_block::unset_mode(char sym) {
	unset_mode_impl(sym);
	on_unset_mode({ sym }); 
}
void mode_block::unset_mode(std::vector<char> syms) {
	for(char sym : syms) unset_mode_impl(sym);
	on_unset_mode(syms); 
}


bool mode_block::is_mode_set(char sym) const {
	return modes.end()==find(sym);
}

optional_string mode_block::try_get_mode_param(char sym) {
	auto it=find(sym);
	if(it==modes.end()) {
		throw std::runtime_error(
			"tring to retreive parater for non existant mode");
	}
	return it->second;
}

std::ostream& operator<<(std::ostream& os, const mode_block& mb) {
	for(auto mode : mb) {
		os << mode.first;
		if(mode.second) os << '(' << *mode.second << ')';
	}
	return os;
}

std::string to_string(const mode_block& mb) {
	std::ostringstream oss;
	oss << mb;
	return oss.str();
}



namespace qi =boost::spirit::qi;

template<typename T>
using rule     =qi::rule<std::string::const_iterator, T(), qi::space_type>;

act_mode parse_modes(const std::string& entries) {

	rule<mode_block::value_type> mode =
		qi::alpha >> -( qi::lexeme[' '] >> +qi::char_ );

	rule<act_mode> modes=qi::char_("+-") >> +mode;

	act_mode mp;
	qi::phrase_parse(entries.begin(), entries.end(), modes, qi::space, mp);
	
	return mp;
	/*
	if(boost::fusion::at_c<0>(mp) == '+') {
		mb.set_mode(boost::fusion::at_c<1>(mp));
	}
	else if(boost::fusion::at_c<0>(mp) == '-') { //TODO deuglify
		std::vector<char> v;
		for(auto& m : boost::fusion::at_c<1>(mp)) 
			v.push_back(m.first);
		mb.unset_mode(v);
	}
	*/
}

} //namespace irc
