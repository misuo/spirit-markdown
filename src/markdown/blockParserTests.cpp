#define BOOST_TEST_MODULE Markdown test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <boost/assign/list_of.hpp>

#include "dom/blocks.hpp"

#include "markdown/blockParser.hpp"

#include "parserPredicates.hpp"
#include "instantiations.hpp"

namespace Markdown {

BOOST_FIXTURE_TEST_SUITE(BlockParserTests, BlockParser<DefaultInputIterator>)

using boost::assign::list_of;
using namespace DOM;

BOOST_AUTO_TEST_CASE(ParaParser)
{
    BOOST_CHECK(Parses(para, "Hello\nThis is a paragraph.\nNot really all that interesting, really.\n"));

    // Need to have the \n in between lines of the para:
    BOOST_CHECK(ParsesTo(para,
                         "Line1\nLine2\n",
                         DOM::Para(list_of<std::string>
                              ("Line1")
                              ("\n")
                              ("Line2")
                             )));

    // LineBreak detection still works:
    BOOST_CHECK(ParsesTo(para,
                         "Line1   \nLine2\n",
                         Para(list_of<Inline>
                              (std::string("Line1"))
                              (LineBreak())
                              (std::string("\n"))
                              (std::string("Line2"))
                             )));
}

BOOST_AUTO_TEST_CASE(BlockquoteParser)
{
    BOOST_CHECK(ParsesTo(blockquote, ">quote\n", Blockquote(list_of(Para("quote")))));

    // Some whitespace after the > is allowed, but shouldn't be included:
    BOOST_CHECK(ParsesTo(blockquote,
                         " > quote\nmorequote\n > stillmore\n",
                         Blockquote(list_of
                                    (Para(list_of<std::string>
                                          ("quote")
                                          ("\n")
                                          ("morequote")
                                          ("\n")
                                          ("stillmore")))
                             )));
    
    auto vlist = list_of<std::string>("verbatim\n")("more verbatim\n");
    // Bit tricky, have to parse the two containing lines as a single verbatim
    BOOST_CHECK(ParsesTo(blockquote, ">    verbatim\n>    more verbatim\n",
                         Blockquote(list_of<Verbatim>( vlist.convert_to_container<std::vector<std::string>>() ))));
//                         Blockquote(list_of<Verbatim>(list_of<std::string>("verbatim\n")("more verbatim\n")))));

    // Blockquote containing a blockquote:
    BOOST_CHECK(ParsesTo(blockquote, ">outer\n>\n>>inner\n",
                         Blockquote(list_of<Block>
                                    (Para("outer"))
                                    (Blockquote(list_of
                                                (Para("inner"))))
                             )));

    // Blockquote containing a list:
    BOOST_CHECK(ParsesTo(blockquote, "> * item1\n> * item2\n",
                         Blockquote(list_of
                                    (UnorderedList(list_of
                                                   (ListItem("item1"))
                                                   (ListItem("item2"))
                                        ))
                             )));

    // Intermingled blank lines are OK:
    BOOST_CHECK(ParsesTo(blockquote, "> quote\n\n> still\n\n> more\n\n> blockquote\n",
                Blockquote(list_of
                           (Para("quote"))
                           (Para("still"))
                           (Para("more"))
                           (Para("blockquote")))
                    ));
}

BOOST_AUTO_TEST_CASE(VerbatimParser)
{
    BOOST_CHECK(ParsesTo(verbatim,
                         "    verbatim [with] *no* changes\n",
                         Verbatim("verbatim [with] *no* changes\n")));

    BOOST_CHECK(ParsesTo(verbatim,
                         "    verbatim\n\tmore verbatim\n",
                         Verbatim(list_of<std::string>
                                  ("verbatim\n")
                                  ("more verbatim\n"))
                    ));

    // Intermingled blank lines are OK:
    BOOST_CHECK(ParsesTo(verbatim,
                         "\tline1\n\n\tline2\n",
                         Verbatim(list_of<std::string>
                                  ("line1\n")
                                  ("\n")
                                  ("line2\n"))
                    ));
}

BOOST_AUTO_TEST_CASE(HTMLBlockParser)
{
    BOOST_CHECK(ParsesTo(htmlBlock,
                         "<ul><li>Don't</li><li>Parse</li><li>Me</li></ul>",
                         HTMLBlock("<ul><li>Don't</li><li>Parse</li><li>Me</li></ul>")));
}

BOOST_AUTO_TEST_CASE(HorizontalRuleParser)
{
    BOOST_CHECK(ParsesTo(horizontalRule, " ***\n\n", HorizontalRule()));
    BOOST_CHECK(Parses(horizontalRule, " - - - \n  \n\n"));
    BOOST_CHECK(Parses(horizontalRule, "  ____ _ _ ___ \n\n"));
    BOOST_CHECK(!Parses(horizontalRule, " **\n\n"));
    BOOST_CHECK(!Parses(horizontalRule, "***\n"));
    BOOST_CHECK(!Parses(horizontalRule, "    **\n\n"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Markdown {
