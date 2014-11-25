#ifndef IRC_PARSE_COLOURED_STRING_HPP
#define IRC_PARSE_COLOURED_STRING_HPP

#include <string>
#include <vector>
#include <array>
#include <bitset>

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


/**
 * The text modifiers that we can recieve from other irc users
 */
enum class text_modifiers : char {
    bold,
    colour,
    italic,
    strikethrough,
    reset,
    underline,
    underline2,
    reverse,
	none
};

constexpr std::size_t max_text_modifiers=
	static_cast<std::size_t>(text_modifiers::none);

/**
 * These are the values that map to the 
 */
constexpr std::array<char, max_text_modifiers> modifier_chars {
	{
		0x02, //bold
		0x03, //color
		0x09, //italic
		0x13, //strike-through
		0x0f, //reset
		0x15, //underline 
		0x1f, //underline
		0x16  //reverse 
	}
};


colours colour_cast(unsigned value);

inline
text_modifiers text_modifier_cast(char value) {
	return static_cast<text_modifiers>(value);
}

std::ostream& operator<<(std::ostream& os, colours val);

struct rich_string {
	std::bitset<max_text_modifiers> mods { 0 };
	colours foreground                   { colours::none };
	colours background                   { colours::none };
	std::string value;

	void set_modifier(text_modifiers, bool);
	bool get_modifier(text_modifiers) const;

	bool get_bold() const;
	bool get_italic() const;
	bool get_strikethrough() const;
	bool get_reset() const;
	bool get_underline() const;
	bool get_reverse() const;
};

using coloured_string = rich_string;

struct text_diff {
	text_modifiers modifier   { text_modifiers::none };
	colours        foreground { colours::none        },
	               background { colours::none        };
	std::string    text;
};

using split_string=std::vector<coloured_string>;

split_string parse_coloured_string(const std::string& raw_msg);

} //namespace irc

#endif //IRC_PARSE_COLOURED_STRING_HPP
