#include <coloured_string_parser.hpp>
#include <parse_coloured_string.hpp>

#include <sstream>

namespace irc {

void rich_string::set_modifier(text_modifiers mod, bool val) {
	assert(! ( mod == text_modifiers::none || mod == text_modifiers::colour));
	mods[static_cast<std::size_t>(mod)]=val;
}

bool rich_string::get_modifier(text_modifiers mod) const {
	return mods[static_cast<std::size_t>(mod)];
}



bool rich_string::get_bold() const {
	return get_modifier(text_modifiers::bold);
}

bool rich_string::get_italic() const {
	return get_modifier(text_modifiers::italic);
}

bool rich_string::get_strikethrough() const {
	return get_modifier(text_modifiers::strikethrough);
}

bool rich_string::get_reset() const {
	return get_modifier(text_modifiers::reset);
}

bool rich_string::get_underline() const {
	return get_modifier(text_modifiers::underline);
}

bool rich_string::get_reverse() const {
	return get_modifier(text_modifiers::reverse);
}

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
	//the running state
	rich_string t;

	//did the parse suceed
	bool r;
	do {
		//the diff from the next chunk
		text_diff td;
		//parse
		r=qi::parse(first, last, parser, td);

		//apply the diff
		if(td.modifier == text_modifiers::none) {
			//do nothing
		}
		else if(td.modifier == text_modifiers::colour) {
			t.foreground=td.foreground;
			t.background=td.background;
		}
		else {
			//flip the modifier, eg if was bold unset
			t.set_modifier(td.modifier, !t.get_modifier(td.modifier));
		}
		t.value=td.text;
		fstr.push_back(t);
	}
	while(first != last && r);

	if(!r)
		throw std::runtime_error(
			"provided string could not be parsed: " + raw_msg);

	return fstr;
}

std::ostream& operator<<(std::ostream& os, colours val) {
	return os << colour_names[ static_cast<unsigned>(val) ];
}

} //namespace irc
