#ifndef IRC_PARSE_COLOURED_STRING_HPP
#define IRC_PARSE_COLOURED_STRING_HPP

#include <string>
#include <vector>
#include <array>

namespace irc {

enum class colours : unsigned { 
	white=0, black, blue, green,
	red, brown, purple, orange,
	yellow, light_green, teal, light_cyan,
	light_blue, pink, grey, light_grey,
	none
}; //enum class colours

constexpr unsigned max_colours=static_cast<unsigned>(colours::none);

//names are required for boost test
constexpr std::array<const char*, max_colours+1> colour_names { {
	"white", "black", "blue", "green",
	"red", "brown", "purple", "orange",
	"yellow", "light_green", "teal", "light_cyan",
	"light_blue", "pink", "grey", "light_grey",
	"none"
} };

colours colour_cast(unsigned value);

std::ostream& operator<<(std::ostream& os, colours val);

struct coloured_string {
	colours foreground;
	colours background;
	std::string value;
};

using split_string=std::vector<coloured_string>;

split_string parse_coloured_string(const std::string& raw_msg);

} //namespace irc

#endif //IRC_PARSE_COLOURED_STRING_HPP
