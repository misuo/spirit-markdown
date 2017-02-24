#include <boost/test/unit_test.hpp>

#include "parserPredicates.hpp"
#include "instantiations.hpp"

#include "markdown/stringParsers.hpp"

namespace Markdown {

BOOST_FIXTURE_TEST_SUITE(StringParserTests, StringParsers<DefaultInputIterator>)

BOOST_AUTO_TEST_CASE(BasicChars)
{
    BOOST_CHECK(Parses(spnl, "    \n    "));
    BOOST_CHECK(!Parses(spnl, "    \n\n    "));

    BOOST_CHECK(Parses(specialChar, "\\"));
    BOOST_CHECK(!Parses(specialChar, "a"));

    BOOST_CHECK(ParsesTo(escapedChar, "\\[", '['));
    BOOST_CHECK(!Parses(escapedChar, "abc\\["));

    BOOST_CHECK(Parses(normalChar, "a"));
    BOOST_CHECK(!Parses(normalChar, "<"));
}

BOOST_AUTO_TEST_CASE(EndLine)
{
    BOOST_CHECK(Parses(lineBreak, "  \n", 1));
}

BOOST_AUTO_TEST_CASE(Entities)
{
    BOOST_CHECK(Parses(entity, "&#xabcd;"));
    BOOST_CHECK(Parses(entity, "&#123;"));
    BOOST_CHECK(Parses(entity, "&lt;"));
    BOOST_CHECK(ParsesTo(entity, "&amp;", std::string("&")));
    BOOST_CHECK(!Parses(entity, "&#xamp;"));
    BOOST_CHECK(!Parses(entity, "&;"));
}

BOOST_AUTO_TEST_CASE(LabelParser)
{
    BOOST_CHECK(Parses(label, "[]"));
    BOOST_CHECK(Parses(label, "[foo]"));
    BOOST_CHECK(Parses(label, "[*foo*]"));
    BOOST_CHECK(Parses(label, "[foo ``bar`` baz]"));
    BOOST_CHECK(Parses(label, "[foo [bar]]"));

    BOOST_CHECK(!Parses(label, "[foo bar] baz"));
    BOOST_CHECK(!Parses(label, "[foo baz"));
}

BOOST_AUTO_TEST_CASE(TitleParser)
{
    BOOST_CHECK(Parses(title, "\"\")", 1));
    BOOST_CHECK(Parses(title, "\"title\"  )", 3));
    BOOST_CHECK(Parses(title, "\'title\'\n", 1));
    BOOST_CHECK(ParsesTo(title, "\'title\'", std::string("title")));
    BOOST_CHECK(ParsesTo(title, "\'hello \"world\"\'", std::string("hello \"world\"")));

    // Apparently titles can have quotes inside...
    BOOST_CHECK(ParsesTo(title, "\"Title with \"quotes\" inside\"", std::string("Title with \"quotes\" inside")));

    // Whoops, forgot the final quote, parse it anyway...
    BOOST_CHECK(Parses(title, "\"title)", 1));

    BOOST_CHECK(!Parses(title, "\"title\n\""));
}

BOOST_AUTO_TEST_CASE(SourceParser)
{
    BOOST_CHECK(Parses(source, "<>"));
    BOOST_CHECK(Parses(source, "<hello>"));
    BOOST_CHECK(Parses(source, "<(hello)>"));
    BOOST_CHECK(Parses(source, "<<hello>>"));
    BOOST_CHECK(Parses(source, "<(hello)<there>>"));
    BOOST_CHECK(Parses(source, "(hello)"));
    BOOST_CHECK(Parses(source, "http://foo.com"));
    BOOST_CHECK(Parses(source, "http://blah.com/(hello)world"));
    BOOST_CHECK(!Parses(source, "<hello"));
    BOOST_CHECK(!Parses(source, "(<hello"));
    BOOST_CHECK(!Parses(source, "(<hello)>"));
}

BOOST_AUTO_TEST_CASE(Strings)
{
    BOOST_CHECK(Parses(nonIndentSpace, " "));
    BOOST_CHECK(Parses(nonIndentSpace, "  "));
    BOOST_CHECK(Parses(nonIndentSpace, "   "));
    BOOST_CHECK(!Parses(nonIndentSpace, "    "));

    BOOST_CHECK(Parses(indent, "    "));
    BOOST_CHECK(Parses(indent, "\t"));
    BOOST_CHECK(!Parses(indent, "  "));

    BOOST_CHECK(Parses(blankLine, "\n"));
    BOOST_CHECK(!Parses(blankLine, "hello\n"));
}

BOOST_AUTO_TEST_CASE(QuotedStrings)
{
    BOOST_CHECK(ParsesTo(quoted, "\"\"", std::string("\"\"")));
    BOOST_CHECK(Parses(quoted, "\'\'"));
    BOOST_CHECK(Parses(quoted, "\"abc\""));
    BOOST_CHECK(ParsesTo(quoted, "\"cba \'abc\'\"", std::string("\"cba \'abc\'\"")));
    BOOST_CHECK(ParsesTo(quoted, "\'abc \"cba\"\'", std::string("\'abc \"cba\"\'")));
    BOOST_CHECK(Parses(quoted, "\'\"abc\"\'"));
    BOOST_CHECK(Parses(quoted, "\"it\'s a nice day\""));
    BOOST_CHECK(!Parses(quoted, "\"hello\' \"world\'"));
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
