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

#include <boost/phoenix/fusion/at.hpp>

#include <string>

#include <parse_coloured_string.hpp>


BOOST_FUSION_ADAPT_STRUCT(
	irc::text_diff,
	(irc::text_modifiers, modifier)
	(irc::colours,        foreground)
	(irc::colours,        background)
	(std::string,         text)
)


namespace irc {

namespace qi=boost::spirit::qi;
namespace phx=boost::phoenix;
namespace fsn=boost::fusion;

constexpr char colour_char=
	modifier_chars[ static_cast<std::size_t>(text_modifiers::colour) ];

template <typename Iterator>
struct coloured_string_parser : qi::grammar<Iterator, text_diff()> {

    template<typename Val>
    using rule=qi::rule<Iterator, Val()>;
	using leader_value_type=fsn::vector<text_modifiers, colours, colours>;


	rule<colours>           colour_g;
	rule<text_diff>         split_string_g;
	rule<std::string>       value_g;
	rule<text_modifiers>    text_modifier_g;
	rule<leader_value_type> leader_g;


	coloured_string_parser() : coloured_string_parser::base_type(split_string_g) {
		qi::uint_type uint_;
		qi::char_type char_;
		qi::lit_type  lit;
		qi::attr_type attr;
		qi::_1_type   _1;
		qi::_1_type   _2;
		qi::_val_type _val;

		auto tmchar=[&](text_modifiers tm) {
			return qi::copy(
				lit( modifier_chars[ static_cast<std::size_t>(tm) ] ) >> attr(tm)
			);
		};

		colour_g      =uint_[ _val=phx::bind(colour_cast, _1) ];

		//match all modifiers except colour
		text_modifier_g=
			( tmchar(text_modifiers::bold)
			| tmchar(text_modifiers::italic)
			| tmchar(text_modifiers::strikethrough)
			| tmchar(text_modifiers::reset)
			| tmchar(text_modifiers::underline)
			| tmchar(text_modifiers::underline2)
			| tmchar(text_modifiers::reverse)
			)
			;

		//string until end or next modifier
		value_g=*( !( text_modifier_g | lit(colour_char) )  >> char_ );

		//parse an modifer information, include colour
		leader_g=
			( text_modifier_g                                  >> attr(colours::none) >> attr(colours::none)
			| lit(colour_char) >> attr(text_modifiers::colour) >> colour_g            >> (',' >> colour_g | attr(colours::none))
			| lit(colour_char) >> attr(text_modifiers::colour) >> attr(colours::none) >> attr(colours::none)
			| attr(text_modifiers::none)                       >> attr(colours::none) >> attr(colours::none)
			)
			;

		split_string_g=
			leader_g
			[
				phx::at_c<0>(_val) = phx::at_c<0>(_1),
				phx::at_c<1>(_val) = phx::at_c<1>(_1),
				phx::at_c<2>(_val) = phx::at_c<2>(_1)
			]
			>> value_g [ phx::at_c<3>(_val) = _2 ];
	}
}; //class coloured_string_parser

} //namespace irc

#endif //IRC_COLOURED_STRING_PARSER_HPP
