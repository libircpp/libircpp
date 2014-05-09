#include <parse_coloured_string.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE coloured_string_test

#include <boost/test/unit_test.hpp>

void test_coloured_string(const irc::coloured_string& cs, const std::string& val, irc::colours fg, irc::colours bg) {
	BOOST_CHECK_EQUAL(cs.value, val);
	BOOST_CHECK_EQUAL(cs.foreground, fg);
	BOOST_CHECK_EQUAL(cs.background, bg);
}

BOOST_AUTO_TEST_CASE(empty) {
	auto res=irc::parse_coloured_string("");
	BOOST_CHECK_EQUAL(res.size(), 0);
}

BOOST_AUTO_TEST_CASE(plain) {
	auto res=irc::parse_coloured_string("hello");
	BOOST_CHECK_EQUAL(res.size(), 1);

	test_coloured_string(res[0], "hello", irc::colours::none, irc::colours::none);
}

BOOST_AUTO_TEST_CASE(foreground) {
	{
		auto res=irc::parse_coloured_string("\x03""0hello");
		BOOST_CHECK_EQUAL(res.size(), 1);
		test_coloured_string(res[0], "hello", irc::colours::white, irc::colours::none);
	}
	{
		auto res=irc::parse_coloured_string("\x03""15hello");
		BOOST_CHECK_EQUAL(res.size(), 1);
		test_coloured_string(res[0], "hello", irc::colours::light_grey, irc::colours::none);
	}
}

BOOST_AUTO_TEST_CASE(background) {
	{
		auto res=irc::parse_coloured_string("\x03""0,0hello");
		BOOST_CHECK_EQUAL(res.size(), 1);
		const auto& elem=res[0];
		test_coloured_string(elem, "hello", irc::colours::white, irc::colours::white);
	}
	{
		auto res=irc::parse_coloured_string("\x03""0,15hello");
		BOOST_CHECK_EQUAL(res.size(), 1);
		test_coloured_string(res[0], "hello", irc::colours::white, irc::colours::light_grey);
	}
	{
		auto res=irc::parse_coloured_string("\x03""15,15hello");
		BOOST_CHECK_EQUAL(res.size(), 1);
		test_coloured_string(res[0], "hello", irc::colours::light_grey, irc::colours::light_grey);
	}
}


BOOST_AUTO_TEST_CASE(closing) {
	auto res=irc::parse_coloured_string("\x03""0,0hello\x03");
	BOOST_CHECK_EQUAL(res.size(), 1);
	test_coloured_string(res[0], "hello", irc::colours::white, irc::colours::white);
}

BOOST_AUTO_TEST_CASE(multi) {
	auto res=irc::parse_coloured_string("the\x03""0quick\x03""brown\x03""1,1fox");
	BOOST_CHECK_EQUAL(res.size(), 4);

	test_coloured_string(res[0], "the", irc::colours::none, irc::colours::none);
	test_coloured_string(res[1], "quick", irc::colours::white, irc::colours::none);
	test_coloured_string(res[2], "brown", irc::colours::none, irc::colours::none);
	test_coloured_string(res[3], "fox", irc::colours::black, irc::colours::black);
}

BOOST_AUTO_TEST_CASE(spaces) {
	auto res=irc::parse_coloured_string("the \x03""0 quick \x03"" brown\x03""1,1  fox\x03\x03""1,1\03""1,1 \x03");
	BOOST_CHECK_EQUAL(res.size(), 7);

	test_coloured_string(res[0], "the ", irc::colours::none, irc::colours::none);
	test_coloured_string(res[1], " quick ", irc::colours::white, irc::colours::none);
	test_coloured_string(res[2], " brown", irc::colours::none, irc::colours::none);
	test_coloured_string(res[3], "  fox", irc::colours::black, irc::colours::black);
	test_coloured_string(res[4], "", irc::colours::none, irc::colours::none);
	test_coloured_string(res[5], "", irc::colours::black, irc::colours::black);
	test_coloured_string(res[6], " ", irc::colours::black, irc::colours::black);
}
