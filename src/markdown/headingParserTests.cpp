#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/utility/base_from_member.hpp>

#include "dom/blocks.hpp"

#include "markdown/blockParser.hpp"

#include "parserPredicates.hpp"
#include "instantiations.hpp"

namespace Markdown {

BOOST_FIXTURE_TEST_SUITE(HeadingParserTests, BlockParser<DefaultInputIterator>)

using DOM::Heading;
using boost::assign::list_of;

BOOST_AUTO_TEST_CASE(ATXHeadingParser)
{
    BOOST_CHECK(ParsesTo(atxHeading, "# H1 #\n", Heading(1, "H1")));
    BOOST_CHECK(Parses(atxHeading, "## H2 *bold* ###\n"));
    BOOST_CHECK(Parses(atxHeading, "### Here is a funky <span class=funky>Heading</span>\n"));
    BOOST_CHECK(Parses(atxHeading, "##### With a [link](http://foo.com)\n"));

    BOOST_CHECK(!Parses(atxHeading, " # H1\n"));
    BOOST_CHECK(!Parses(atxHeading, "####### H7\n"));
}

BOOST_AUTO_TEST_CASE(SetextHeadingParser)
{
    BOOST_CHECK(ParsesTo(setextHeading, "yo\n=====\n", Heading(1, "yo")));
    BOOST_CHECK(ParsesTo(setextHeading, "Dude\n---\n", Heading(2, "Dude")));
    BOOST_CHECK(Parses(setextHeading, "*H2*\n---\n"));
    BOOST_CHECK(Parses(setextHeading, " H2 #\n-----\n"));

    BOOST_CHECK(!Parses(heading, "H2\n-=--\n"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
