#include <coloured_string_parser.hpp>

namespace irc {

split_string parse_coloured_string(const std::string& raw_msg) {
	auto first=begin(raw_msg), last=end(raw_msg);

	coloured_string_parser<decltype(first)> parser;
	split_string fstr;

	bool r=qi::parse(first, last, parser, fstr);

	if(!r) 
		throw std::runtime_error(
			"provided string could not be parsed: " + raw_msg);

	return fstr;
}

} //namespace irc
