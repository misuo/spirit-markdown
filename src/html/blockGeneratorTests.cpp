#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include "dom/blocks.hpp"

#include "html/blockGenerator.hpp"

#include "generatorPredicates.hpp"

namespace HTML {

struct BlockGeneratorFixture
: private boost::base_from_member<DOM::ReferenceMap>
, public BlockGenerator<DefaultOutputIterator>
{
    BlockGeneratorFixture()
    : BlockGenerator<DefaultOutputIterator>(boost::cref(boost::base_from_member<DOM::ReferenceMap>::member)) {}
};

BOOST_FIXTURE_TEST_SUITE(BlockGeneratorTests, BlockGeneratorFixture)

using namespace DOM;
using boost::assign::list_of;

BOOST_AUTO_TEST_CASE(ParaGenerator)
{
    BOOST_CHECK(Generates(para, Para("G\'day world!"), "<p>G\'day world!</p>\n"));

    BOOST_CHECK(Generates(para,
                          Para(list_of<Inline>
                               (std::string("foo "))
                               (Emph("bar"))),
                          "<p>foo <em>bar</em></p>\n"));
}

BOOST_AUTO_TEST_CASE(BlockquoteGenerator)
{
    BOOST_CHECK(Generates(blockquote, Blockquote("G\'day world!"), "<blockquote>\n<p>G\'day world!</p>\n</blockquote>\n"));

    BOOST_CHECK(Generates(blockquote,
                          Blockquote(list_of
                                     (Verbatim("Wazzup!"))),
                          "<blockquote>\n<pre><code>Wazzup!</code></pre>\n</blockquote>\n"));
}

BOOST_AUTO_TEST_CASE(VerbatimGenerator)
{
    BOOST_CHECK(Generates(verbatim, Verbatim("this & that\n"), "<pre><code>this &amp; that\n</code></pre>\n"));

    BOOST_CHECK(Generates(verbatim,
                          Verbatim(list_of
                                   (std::string("Massive Attack\n"))
                                   (std::string("Atlas Air\n"))),
                          "<pre><code>Massive Attack\nAtlas Air\n</code></pre>\n"));
}

BOOST_AUTO_TEST_CASE(HeadingGenerator)
{
    BOOST_CHECK(Generates(heading, Heading(1, "My Heading"), "<h1>My Heading</h1>\n"));
    
    BOOST_CHECK(Generates(heading, Heading(2, list_of<Inline>(std::string("My "))(Emph("Funky Heading"))),
                          "<h2>My <em>Funky Heading</em></h2>\n"));
}

BOOST_AUTO_TEST_CASE(ListGenerator)
{
    BOOST_CHECK(Generates(orderedList, OrderedList("one item"), "<ol>\n<li>one item</li>\n</ol>\n"));
}

BOOST_AUTO_TEST_CASE(HTMLBlockGenerator)
{
    BOOST_CHECK(Generates(htmlBlock, HTMLBlock("<p>Hello</p>"), "<p>Hello</p>\n"));

    // Shouldn't do any escaping
    BOOST_CHECK(Generates(htmlBlock, HTMLBlock("<p>6 > 5</p>"), "<p>6 > 5</p>\n"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace HTML {
