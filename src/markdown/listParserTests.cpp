#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

#include "markdown/blockParser.hpp"

#include "parserPredicates.hpp"
#include "instantiations.hpp"

namespace Markdown {

BOOST_FIXTURE_TEST_SUITE(ListParserTests, BlockParser<DefaultInputIterator>)

using namespace DOM;
using boost::assign::list_of;

BOOST_AUTO_TEST_CASE(ListParser)
{
    // Basic tight list
    //
    //  * TightItem1
    //  * <http://example.com>
    //  * Item3
    //  
    BOOST_CHECK(ParsesTo(unorderedList, " * TightItem1\n * <http://example.com>\n * Item3\n",
                         UnorderedList(list_of
                                       (ListItem("TightItem1"))
                                       (ListItem(list_of<Inline>
                                                 (AutoLinkURL("http://example.com"))))
                                       (ListItem("Item3"))
                             )));

    // Sub-list parsing
    //
    // *   Item1
    //     *   Item1.1
    //     *   Item1.2
    // *   Item2
    //     *   Item2.1
    // 
    BOOST_CHECK(ParsesTo(unorderedList,
                         "*   Item1\n    *   Item1.1\n    *   Item1.2\n*   Item2\n    *   Item2.1\n",
                         UnorderedList(list_of
                                       (ListItem(list_of<std::string>
                                                 ("Item1"),
                                                 list_of<Block>
                                                 (UnorderedList(list_of
                                                                (ListItem("Item1.1"))
                                                                (ListItem("Item1.2"))
                                                     ))
                                           ))
                                       (ListItem(list_of<std::string>
                                                 ("Item2"),
                                                 list_of<Block>
                                                 (UnorderedList(list_of
                                                                (ListItem("Item2.1"))
                                                     ))
                                           ))
                             )));


    BOOST_CHECK(Parses(unorderedList, "* Tight Item 1\n   More Item 1\nStill More Item 1\n* Tight Item 2\n* The End\n"));

    // Basic continuation block
    //
    // * OneItem
    //
    //     WithContinuationBlock
    // 
    BOOST_CHECK(ParsesTo(unorderedList,
                         " * OneItem\n\n    WithContinuationBlock\n",
                         UnorderedList(list_of
                                       (ListItem(list_of<std::string>("OneItem"),
                                                 list_of<Block>
                                                 (Para("WithContinuationBlock"))
                                           )))
                    ));

    // Loose List
    //
    // + Item1
    //
    // + Item2
    //
    // + Item3
    // 
    BOOST_CHECK(ParsesTo(unorderedList,
                         " + Item1\n\n + Item2\n\n + Item3\n",
                         UnorderedList(list_of
                                       (ListItem("Item1"))
                                       (ListItem("Item2"))
                                       (ListItem("Item3")),
                                       false // loose
                             )
                    ));

    // Loose list with multi-line items
    //
    // * LooseItem1
    // MoreItem1
    //
    // * Item2
    //
    // * Item3
    // 
    BOOST_CHECK(ParsesTo(unorderedList,
                         " * LooseItem1\nMoreItem1\n\n * Item2\n\n * Item3\n",
                         UnorderedList(list_of
                                       (ListItem(list_of<std::string>
                                                 ("LooseItem1")
                                                 ("\n")
                                                 ("MoreItem1")))
                                       (ListItem("Item2"))
                                       (ListItem("Item3")),
                                       false
                             )
                    ));;

    BOOST_CHECK(Parses(orderedList, " 1. Tight Item 1\n 1. Item 2\n 1. Item 3\n"));
    BOOST_CHECK(Parses(orderedList, " 1. Tight Item 1\n 1. Item 2\n 1. Item 3\n\n\n    Continuation Block\n"));
    BOOST_CHECK(Parses(orderedList, " 1. Tight Item 1\nMore Item 1\n\n 1. Item 2\n"));

    // * Item 1
    //
    //     * Item 1.1
    //     * Item 1.2
    //
    //  * Item 2
    //  
    BOOST_CHECK(Parses(unorderedList, " * Item 1\n\n\t * Item 1.1\n\t * Item 1.2\n\n * Item 2\n"));

    // * Item 1
    //     * Item 1.1
    //         * Item 1.1.1
    // 
    BOOST_CHECK(Parses(unorderedList, " * Item 1\n\t * Item 1.1\n\t\t * Item 1.1.1\n"));

    // Nasty corner case from MarkdownTest:
    // 
    // *    this
    // 
    //     *    sub
    //     
    //     that
    //     
    BOOST_CHECK(Parses(unorderedList, "*\tthis\n\n\t*\tsub\n\n\tthat\n"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
