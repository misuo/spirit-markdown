#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include "dom/inlines.hpp"

#include "html/inlineGenerator.hpp"

#include "generatorPredicates.hpp"

namespace HTML {

using boost::assign::list_of;

struct InlineGeneratorFixture: protected boost::base_from_member<DOM::ReferenceMap>, public InlineGenerator<DefaultOutputIterator>
{
    typedef boost::base_from_member<DOM::ReferenceMap> ReferenceMap_base;

    InlineGeneratorFixture() : InlineGenerator<DefaultOutputIterator>(boost::cref(ReferenceMap_base::member)) {}
};

BOOST_FIXTURE_TEST_SUITE(InlineGeneratorTests, InlineGeneratorFixture)

using namespace DOM;

BOOST_AUTO_TEST_CASE(TextGenerator)
{
    BOOST_CHECK(Generates(text, std::string("hello"), "hello"));
    BOOST_CHECK(Generates(text, std::string("AT&T"), "AT&amp;T"));
}

BOOST_AUTO_TEST_CASE(EmphStrongGenerator)
{
    BOOST_CHECK(Generates(emph, Emph("hello"), "<em>hello</em>"));
    BOOST_CHECK(Generates(emph, Emph("D&B"), "<em>D&amp;B</em>"));
    BOOST_CHECK(Generates(strong, Strong("blackberries"), "<strong>blackberries</strong>"));

    BOOST_CHECK(Generates(emph, Emph(list_of<Inline>(std::string("hello "))(Strong("world"))), "<em>hello <strong>world</strong></em>"));
}

BOOST_AUTO_TEST_CASE(InlineHTMLGenerator)
{
    BOOST_CHECK(Generates(inlineHTML, InlineHTML("<span foo=\"a&b\">notescaped</span>"), "<span foo=\"a&b\">notescaped</span>"));
}

BOOST_AUTO_TEST_CASE(CodeGenerator)
{
    BOOST_CHECK(Generates(code, Code("hello"), "<code>hello</code>"));
    BOOST_CHECK(Generates(code, Code("B&W"), "<code>B&amp;W</code>"));
    //BOOST_CHECK(Generates(inline_, Code("B&W"), "<code>B&amp;W</code>")); // 2017-02-23 Temp outcommented.
}

BOOST_AUTO_TEST_CASE(ExplicitLinkGenerator)
{
    BOOST_CHECK(Generates(explicitLink, ExplicitLink(false, "Simple", "http://link.com", boost::none), "<a href=\"http://link.com\">Simple</a>"));

    // Title has quotes, needs escaping
    BOOST_CHECK(Generates(explicitLink, ExplicitLink(false, "Foo", "http://bar", std::string("\"baz\"")), "<a href=\"http://bar\" title=\"&quot;baz&quot;\">Foo</a>"));

    BOOST_CHECK(Generates(explicitLink, ExplicitLink(true, "Img", "http://yay", std::string("hello")), "<img src=\"http://yay\" title=\"hello\">Img</img>"));
}

BOOST_AUTO_TEST_CASE(ReferenceLinkGenerator)
{
    ReferenceMap_base::member.insert(Reference(false, "ref", "http://foo.com", boost::none));

    BOOST_CHECK(Generates(referenceLink, ReferenceLink(false, "Label", std::string("ref")), "<a href=\"http://foo.com\">Label</a>"));

    BOOST_CHECK(Generates(referenceLink, ReferenceLink(false, "ref", boost::none), "<a href=\"http://foo.com\">ref</a>"));

    // Missing reference
    BOOST_CHECK(Generates(referenceLink, ReferenceLink(false, "foo", boost::none), "[foo]"));
}

BOOST_AUTO_TEST_CASE(AutoLinkGenerator)
{
    BOOST_CHECK(Generates(autoLinkEmail, AutoLinkEmail("foo@bar.com"), "<a href=\"mailto:foo@bar.com\">foo@bar.com</a>"));

    BOOST_CHECK(Generates(autoLinkURL, AutoLinkURL("http://foo.com"), "<a href=\"http://foo.com\">http://foo.com</a>"));

    BOOST_CHECK(Generates(autoLinkURL, AutoLinkURL("http://foo.com/?foo=1&bar=2"), "<a href=\"http://foo.com/?foo=1&amp;bar=2\">http://foo.com/?foo=1&amp;bar=2</a>"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace HTML {
