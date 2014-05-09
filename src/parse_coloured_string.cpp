#include <coloured_string_parser.hpp>
#include <parse_coloured_string.hpp>

#include <sstream>

namespace irc {

colours colour_cast(unsigned value) {
	if(value > max_colours) {
		std::ostringstream oss;
		oss << "Value not a valid enum value: " << value 
		    << " max value is: " << max_colours;
		throw std::runtime_error(oss.str());
	}
	return static_cast<colours>(value);
}

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

std::ostream& operator<<(std::ostream& os, colours val) {
	return os << colour_names[static_cast<unsigned>(val)];
}

} //namespace irc
