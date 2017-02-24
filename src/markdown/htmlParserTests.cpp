#include <boost/test/unit_test.hpp>
#include <boost/utility/base_from_member.hpp>

#include "parserPredicates.hpp"
#include "instantiations.hpp"

#include "markdown/stringParsers.hpp"
#include "markdown/htmlParsers.hpp"

namespace Markdown {

struct HTMLParserFixture
: private boost::base_from_member<StringParsers<DefaultInputIterator> >
, public HTMLParsers<DefaultInputIterator>
{
    HTMLParserFixture()
    : HTMLParsers<DefaultInputIterator>(boost::base_from_member<StringParsers<DefaultInputIterator> >::member) {}
};

BOOST_FIXTURE_TEST_SUITE(HTMLParserTests, HTMLParserFixture)

BOOST_AUTO_TEST_CASE(HTMLAttributes)
{
    BOOST_CHECK(Parses(HTMLAttribute, "foo"));
    BOOST_CHECK(Parses(HTMLAttribute, "empty=\"\""));
    BOOST_CHECK(Parses(HTMLAttribute, "foo=\"bar\""));
    BOOST_CHECK(Parses(HTMLAttribute, "foo = bar"));
    BOOST_CHECK(Parses(HTMLAttribute, "  bar = \n  \"baz\""));
    BOOST_CHECK(Parses(HTMLAttribute, "foo-bar \n \n  =\"baz \'boz\'\""));
    BOOST_CHECK(!Parses(HTMLAttribute, "foo+bar"));
    BOOST_CHECK(!Parses(HTMLAttribute, "foo="));
}

BOOST_AUTO_TEST_CASE(HTMLComments)
{
    BOOST_CHECK(Parses(HTMLComment, "<!-- hi -->"));
    BOOST_CHECK(!Parses(HTMLComment, "<!-- --> -->"));
}

BOOST_AUTO_TEST_CASE(EmptyInlineElements)
{
    BOOST_CHECK(Parses(emptyInlineElement, "<foo/>"));
    BOOST_CHECK(Parses(emptyInlineElement, "< foo / >"));
    BOOST_CHECK(Parses(emptyInlineElement, "<foo bar =  baz  />"));
    BOOST_CHECK(Parses(emptyInlineElement, "<foo bar = \"baz\" fish = face />"));
    BOOST_CHECK(Parses(emptyInlineElement, "<foo bar=\'baz\'\n bar=\"baz\"\n/>"));
    BOOST_CHECK(Parses(emptyInlineElement, "<foo />"));

    BOOST_CHECK(!Parses(emptyInlineElement, "<foo"));
}

BOOST_AUTO_TEST_CASE(NonEmptyInlineElements)
{
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<foo>bar</foo>"));
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<foo bar =  baz   >hello <abc/>, <def>ghi</def> </foo>"));
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<foo bar = \"baz\" >la la lah< / foo >"));
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<span class=\"funky\">Heading</span>"));
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<span attr='`ticks`'>like this</span>"));
    BOOST_CHECK(Parses(nonEmptyInlineElement, "<foo bar=\'baz\'\n bar=\"baz\"\n>\nhello<\n/\nfoo >"));

    BOOST_CHECK(!Parses(nonEmptyInlineElement, "<foo><bar></foo>"));
    BOOST_CHECK(!Parses(nonEmptyInlineElement, "<p>hello</p>"));
    BOOST_CHECK(!Parses(nonEmptyInlineElement, "<fish/>hello</fish>"));
}

BOOST_AUTO_TEST_CASE(BlockElements)
{
    BOOST_CHECK(Parses(blockElement, "<P>foo</p>"));
    BOOST_CHECK(Parses(blockElement, "<p>foo<b>bar</b></p>"));
    BOOST_CHECK(Parses(blockElement, "<p><ul><li><p>hello</p></li></ul></p>"));
    BOOST_CHECK(Parses(blockElement, "<p><div>hello<hr/>goodbye</div></p>"));
    BOOST_CHECK(Parses(blockElement, "<p><!-- </p> --></p>"));

    BOOST_CHECK(!Parses(blockElement, "<b>foo</b>"));
    BOOST_CHECK(!Parses(blockElement, "<b>foo</b> bar"));
    BOOST_CHECK(!Parses(blockElement, "<p>foo<div>bar</p></div>"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
