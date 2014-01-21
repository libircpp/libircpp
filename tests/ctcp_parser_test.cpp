#define BOOST_TEST_MODULE ctcp_parser_test

#include <ctcp.hpp>
#include <ctcp_parser.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/test/minimal.hpp>

#include <iostream>

namespace ctcp = irc::ctcp;
namespace fsn  = boost::fusion;
namespace qi   = boost::spirit::qi;

int test_main(int, char **) {

    bool success;
    qi::space_type space;
    fsn::vector<ctcp::command, std::string>  result;
    std::string test_str = "\001VERSION a v1 b\001";
    std::string::const_iterator first = test_str.begin();
    std::string::const_iterator last  = test_str.end();
    irc::ctcp_parser<std::string::const_iterator> ctcp_parser;

    success = qi::phrase_parse( first, last, ctcp_parser, space, result );
    ctcp::command cmd = fsn::at_c<0>(result);
    std::string   str = fsn::at_c<1>(result);

    BOOST_CHECK(success);
    BOOST_CHECK(cmd == ctcp::command::version);
    BOOST_CHECK(str == "a v1 b");

    test_str = "\001SOURCE\001";
    first    = test_str.begin();
    last     = test_str.end();
    success  = qi::phrase_parse( first, last, ctcp_parser, space, result );
    cmd      = fsn::at_c<0>(result);
    str      = fsn::at_c<1>(result);

    BOOST_CHECK(success);
    BOOST_CHECK(cmd == ctcp::command::source);
    BOOST_CHECK(str == std::string{});

    test_str = "fake";
    first    = test_str.begin();
    last     = test_str.end();
    success  = qi::phrase_parse( first, last, ctcp_parser, space, result );
    cmd      = fsn::at_c<0>(result);
    str      = fsn::at_c<1>(result);

    BOOST_CHECK(success);
    BOOST_CHECK(cmd == ctcp::command::none);
    BOOST_CHECK(str == std::string{});

    test_str = "\001PANG :a b c\001";
    first    = test_str.begin();
    last     = test_str.end();
    success  = qi::phrase_parse( first, last, ctcp_parser, space, result );
    cmd      = fsn::at_c<0>(result);
    str      = fsn::at_c<1>(result);

    BOOST_CHECK(success);
    BOOST_CHECK(cmd == ctcp::command::none);
    BOOST_CHECK(str == std::string{});

    test_str = "\001TIME :Thu Aug 11 21:52:51 1994 CST\001";
    first    = test_str.begin();
    last     = test_str.end();
    success  = qi::phrase_parse( first, last, ctcp_parser, space, result );
    cmd      = fsn::at_c<0>(result);
    str      = fsn::at_c<1>(result);

    BOOST_CHECK(success);
    BOOST_CHECK(cmd == ctcp::command::time);
    BOOST_CHECK(str == ":Thu Aug 11 21:52:51 1994 CST");

    return 0;
}
