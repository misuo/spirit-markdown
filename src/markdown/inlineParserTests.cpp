#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/utility/base_from_member.hpp>

#include "dom/inlines.hpp"

#include "markdown/stringParsers.hpp"
#include "markdown/htmlParsers.hpp"
#include "markdown/inlineParsers.hpp"

#include "parserPredicates.hpp"
#include "instantiations.hpp"

namespace Markdown {

using boost::assign::list_of;

struct InlineParserFixtures
    : private boost::base_from_member<StringParsers<DefaultInputIterator> >
    , private boost::base_from_member<HTMLParsers<DefaultInputIterator> >
    , public InlineParsers<DefaultInputIterator>
{
    typedef boost::base_from_member<StringParsers<DefaultInputIterator> > StringParsers_type;
    typedef boost::base_from_member<HTMLParsers<DefaultInputIterator> > HTMLParsers_type;

    InlineParserFixtures()
        : HTMLParsers_type(boost::cref(StringParsers_type::member))
        , InlineParsers<DefaultInputIterator>(StringParsers_type::member, HTMLParsers_type::member) {}
};

BOOST_FIXTURE_TEST_SUITE(InlineParserTests, InlineParserFixtures)

BOOST_AUTO_TEST_CASE(StarLineParser)
{
    BOOST_CHECK(Parses(starLine, "****"));
    BOOST_CHECK(Parses(starLine, "*******"));
    BOOST_CHECK(Parses(starLine, " **** ", 1));
}

BOOST_AUTO_TEST_CASE(EmphStrongParser)
{
    BOOST_CHECK(ParsesTo(emph, "*emph*", DOM::Emph("emph")));
    BOOST_CHECK(!Parses(emph, "**emph*"));
    BOOST_CHECK(!Parses(emph, "*emph**"));
    BOOST_CHECK(ParsesTo(emph, "_emph_", DOM::Emph("emph")));
    BOOST_CHECK(!Parses(emph, "__emph_"));
    BOOST_CHECK(!Parses(emph, "_emph__"));

    BOOST_CHECK(ParsesTo(strong, "**strong**", DOM::Strong("strong")));
    BOOST_CHECK(!Parses(strong, "**strong*"));
    BOOST_CHECK(!Parses(strong, "*strong**"));
    BOOST_CHECK(ParsesTo(strong, "__strong__", DOM::Strong("strong")));
    BOOST_CHECK(!Parses(strong, "__strong_"));
    BOOST_CHECK(!Parses(strong, "_strong__"));
}

BOOST_AUTO_TEST_CASE(InlineHTMLParser)
{
    BOOST_CHECK(ParsesTo(inlineHTML, "<foo/>", DOM::InlineHTML("<foo/>")));
    BOOST_CHECK(ParsesTo(inlineHTML, "<span foo= \"bar\" >baz</span>", DOM::InlineHTML("<span foo= \"bar\" >baz</span>")));
    BOOST_CHECK(ParsesTo(inlineHTML, "<!-- foo -->", DOM::InlineHTML("<!-- foo -->")));

    BOOST_CHECK(!Parses(inlineHTML, "foo"));
}

BOOST_AUTO_TEST_CASE(CodeParser)
{
    BOOST_CHECK(ParsesTo(code, "`code`", DOM::Code("code")));
    BOOST_CHECK(ParsesTo(code, "``code``", DOM::Code("code")));
    BOOST_CHECK(ParsesTo(code, "``co`de``", DOM::Code("co`de")));
    BOOST_CHECK(ParsesTo(code, "````code````", DOM::Code("code")));
    BOOST_CHECK(ParsesTo(code, "````co```de````", DOM::Code("co```de")));
    BOOST_CHECK(ParsesTo(code, "`` `backticks` ``", DOM::Code(" `backticks` ")));
    BOOST_CHECK(Parses(code, "`<test a=\"` content of attribute `\">`"));

    BOOST_CHECK(!Parses(code, "``"));
    BOOST_CHECK(!Parses(code, "``code`"));
    BOOST_CHECK(!Parses(code, "``co``de`"));
}

BOOST_AUTO_TEST_CASE(ExplicitLinkParser)
{
    BOOST_CHECK(ParsesTo(explicitLink,
                         "[Foo] (http://bar \"baz\")",
                         DOM::ExplicitLink(false, "Foo", "http://bar", std::string("baz"))));

    BOOST_CHECK(ParsesTo(explicitLink,
                         "[Hello] (http://hello)",
                         DOM::ExplicitLink(false, "Hello", "http://hello", boost::none)));

    BOOST_CHECK(ParsesTo(explicitLink,
                         "![Img](<http://hello> \"There\")",
                         DOM::ExplicitLink(true, "Img", "http://hello", std::string("There"))));
}

BOOST_AUTO_TEST_CASE(ReferenceLinkParser)
{
    BOOST_CHECK(ParsesTo(referenceLink,
                         "[Hello][there]",
                         DOM::ReferenceLink(false, "Hello", std::string("there"))));

    BOOST_CHECK(ParsesTo(referenceLink,
                         "![Hello]\n[1]",
                         DOM::ReferenceLink(true, "Hello", std::string("1"))));

    BOOST_CHECK(ParsesTo(referenceLink,
                         "[Hello]",
                         DOM::ReferenceLink(false, "Hello", boost::none)));

    BOOST_CHECK(ParsesTo(referenceLink,
                         "![Hello][]",
                         DOM::ReferenceLink(true, "Hello", boost::none)));
}

BOOST_AUTO_TEST_CASE(AutolinkParser)
{
    BOOST_CHECK(ParsesTo(autolinkEmail, "<foo@bar>", DOM::AutoLinkEmail("foo@bar")));

    BOOST_CHECK(ParsesTo(autolinkUrl, "<http://foo.bar>", DOM::AutoLinkURL("http://foo.bar")));

    BOOST_CHECK(!Parses(autolinkEmail, "<h77p://foo.bar>"));
    BOOST_CHECK(!Parses(autolinkUrl, "<h77p://foo.bar>"));
    BOOST_CHECK(!Parses(autolinkEmail, "<foobar>"));
    BOOST_CHECK(!Parses(autolinkUrl, "<foobar>"));
}

BOOST_AUTO_TEST_CASE(InlineSequenceParser)
{
    {
        const char testString[] = "Hello *world*<span foo= \"bar\" >baz</span>[link](http://me.com)";
        DOM::Inlines expected(
            list_of<DOM::Inline>
            (std::string("Hello"))
            (std::string(" "))
            (DOM::Emph("world"))
            (DOM::InlineHTML("<span foo= \"bar\" >baz</span>"))
            (DOM::ExplicitLink(false, "link", "http://me.com", boost::none))
            );
        BOOST_CHECK(ParsesTo(inlines, testString, expected));
    }
    
    {
        const char testString[] = "4 < 5.";
        DOM::Inlines expected(
            list_of<DOM::Inline>
            (std::string("4"))
            (std::string(" "))
            (std::string("<"))
            (std::string(" "))
            (std::string("5."))
            );
        BOOST_CHECK(ParsesTo(inlines, testString, expected));
    }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
