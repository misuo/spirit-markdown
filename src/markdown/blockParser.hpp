#ifndef MARKDOWN_BLOCK_PARSER_HPP
#define MARKDOWN_BLOCK_PARSER_HPP

#include "dom/blockForwards.hpp"

#include "markdown/stringParsers.hpp"
#include "markdown/inlineParsers.hpp"
#include "markdown/htmlParsers.hpp"

namespace Markdown {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct BlockParser : public qi::grammar<Iterator, DOM::Block()>
{
    BlockParser();

    StringParsers<Iterator> strings;
    HTMLParsers<Iterator> html;
    InlineParsers<Iterator> inline_;

    // Headings

    qi::rule<Iterator, unsigned()> hashes;
    qi::rule<Iterator> trailingHashes;
    qi::rule<Iterator, unsigned(char, unsigned)> setextUnderline;
    qi::rule<Iterator, DOM::Heading()> atxHeading, setextHeading, heading;

    // Lists

    qi::rule<Iterator, std::vector<DOM::Inline>(std::string), qi::locals<std::string> > listItemContents;
    qi::rule<Iterator, DOM::Block(std::string), qi::locals<std::string> > listItemContinuation;

    qi::rule<Iterator> bullet;
    qi::rule<Iterator, DOM::ListItem(std::string)> unorderedListItem;
    qi::rule<Iterator, DOM::UnorderedList(std::string), qi::locals<bool> > unorderedSubList;
    qi::rule<Iterator, DOM::UnorderedList()> unorderedList;

    qi::rule<Iterator> enumerator;
    qi::rule<Iterator, DOM::ListItem(std::string)> orderedListItem;
    qi::rule<Iterator, DOM::OrderedList(std::string), qi::locals<bool> > orderedSubList;
    qi::rule<Iterator, DOM::OrderedList()> orderedList;

    // Other Blocks

    qi::rule<Iterator, DOM::Para()> para;
    qi::rule<Iterator, DOM::Para(std::string), qi::locals<std::string> > subPara;

    qi::rule<Iterator, DOM::Blockquote()> blockquote;
    qi::rule<Iterator, DOM::Blockquote(std::string), qi::locals<std::string> > subBlockquote;

    qi::rule<Iterator, DOM::Verbatim()> verbatim;
    qi::rule<Iterator, DOM::Verbatim(std::string), qi::locals<std::string> > subVerbatim;

    qi::rule<Iterator, DOM::HorizontalRule()> horizontalRule;

    qi::rule<Iterator, DOM::HTMLBlock()> htmlBlock;

    // Finally the Block parser
    
    qi::rule<Iterator, DOM::Block()> block;

private:
    // The list and heading parsers contain many complex rules, requiring lots of memory at compile
    // time. We put these into separate compilation units to minimise this overhead...
    void constructListParsers();
    void constructHeadingParsers();
};

} // namespace Markdown {

#endif // MARKDOWN_BLOCK_PARSER_HPP
