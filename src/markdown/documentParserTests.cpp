#include <boost/test/unit_test.hpp>

#include "dom/blocks.hpp"
#include "dom/referenceMap.hpp"

#include "markdown/documentParser.hpp"

#include "parserPredicates.hpp"
#include "instantiations.hpp"

namespace Markdown {

struct DocumentParserFixtures
: protected boost::base_from_member<DOM::ReferenceMap>
, public DocumentParser<DefaultInputIterator>
{
    typedef boost::base_from_member<DOM::ReferenceMap> ReferenceMap_base;

    DocumentParserFixtures()
    : DocumentParser<DefaultInputIterator>(boost::ref(ReferenceMap_base::member)) {}
};

BOOST_FIXTURE_TEST_SUITE(DocumentParserTests, DocumentParserFixtures)

using DOM::Reference;

BOOST_AUTO_TEST_CASE(ReferenceParser)
{
    BOOST_CHECK(ParsesTo(reference, " [ref]: hello\n", Reference(false, "ref", "hello", boost::none)));
    BOOST_CHECK(ParsesTo(reference, "   [ref]: hello\n\'world\'\n", Reference(false, "ref", "hello", std::string("world"))));
    BOOST_CHECK(ParsesTo(reference, "[2]: http://att.com/  \"AT&T\"\n", Reference(false, "2", "http://att.com/", std::string("AT&T"))));
    BOOST_CHECK(ParsesTo(reference, "  [bar]: /url/ \"Title with \"quotes\" inside\"\n", Reference(false, "bar", "/url/", std::string("Title with \"quotes\" inside"))));
    BOOST_CHECK(!Parses(reference, " []:abc \'hello\'\n"));
}

BOOST_AUTO_TEST_CASE(PopulatesReferenceMap)
{
    BOOST_CHECK(Parses(document, "[ref]: http://blah.com\n"));

    // We should have added the reference to the reference map:
    BOOST_CHECK(ReferenceMap_base::member.count("ref") == 1);
    BOOST_CHECK_EQUAL(*ReferenceMap_base::member.find("ref"), Reference(false, "ref", "http://blah.com", boost::none));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
