//          Copyright Joseph Dobson, Andrea Zanellato 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IRC_COLOURED_STRING_PARSER_HPP
#define IRC_COLOURED_STRING_PARSER_HPP

#define BOOST_RESULT_OF_USE_DECLTYPE         1
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1
#define BOOST_SPIRIT_USE_PHOENIX_V3          1

#include <boost/spirit/home/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <string>
#include <vector>

#include <parse_coloured_string.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	irc::coloured_string,
	(irc::colours, foreground)
	(irc::colours, background)
	(std::string, value)
)

namespace irc {

namespace qi=boost::spirit::qi;
namespace phx=boost::phoenix;

using split_string=std::vector<coloured_string>;
constexpr char colour_char=0x03;

template <typename Iterator>
struct coloured_string_parser : qi::grammar<Iterator, split_string()> {

    template<typename Val>
    using rule=qi::rule<Iterator, Val()>;

	rule<colours>      colour_g;
	rule<split_string> split_string_g;
	rule<std::string>  value_g;

	coloured_string_parser() : coloured_string_parser::base_type(split_string_g) {
		qi::uint_type uint_;
		qi::char_type char_;
		qi::lit_type  lit;
		qi::attr_type attr;
		qi::_1_type   _1;
		qi::_val_type _val;

		colour_g      =uint_[ _val=phx::bind(colour_cast, _1) ];

		value_g = +~char_(colour_char) | attr(std::string{}) >> &lit(colour_char);

		split_string_g=
			*( colour_char >> colour_g            >> (',' >> colour_g | attr(colours::none)) >> value_g
			 | colour_char >> attr(colours::none) >> attr(colours::none)                     >> value_g
			 | attr(colours::none)                >> attr(colours::none)                     >> +~char_(colour_char)
			 )
			 ;
	}
}; //class coloured_string_parser

} //namespace irc

#endif //IRC_COLOURED_STRING_PARSER_HPP
