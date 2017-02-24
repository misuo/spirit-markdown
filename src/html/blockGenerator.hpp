#ifndef HTML_BLOCK_GENERATOR_HPP
#define HTML_BLOCK_GENERATOR_HPP

#include <boost/spirit/include/karma.hpp>

#include "dom/blockForwards.hpp"

#include "html/inlineGenerator.hpp"

namespace HTML {

namespace karma = boost::spirit::karma;

template <typename Iterator>
struct BlockGenerator : public karma::grammar<Iterator, DOM::Block()>
{
    BlockGenerator(const DOM::ReferenceMap& refmap_);

    InlineGenerator<Iterator> inline_;

    typedef boost::fusion::result_of::as_nview<DOM::Heading const, 0, 1, 0>::type HeadingReordered;

    karma::rule<Iterator, DOM::Para()> para;
    karma::rule<Iterator, DOM::Blockquote()> blockquote;
    karma::rule<Iterator, HeadingReordered()> headingReordered;
    karma::rule<Iterator, DOM::Heading()> heading;
    karma::rule<Iterator, DOM::Verbatim()> verbatim;
    karma::rule<Iterator, DOM::HorizontalRule()> horizontalRule;
    karma::rule<Iterator, DOM::HTMLBlock()> htmlBlock;

    karma::rule<Iterator, DOM::ListItem(bool)> listItem;
    karma::rule<Iterator, DOM::UnorderedList()> unorderedList;
    karma::rule<Iterator, DOM::OrderedList()> orderedList;

    karma::rule<Iterator, DOM::Block()> block;
};
        
} // namespace HTML {

#endif // BLOCK_HTML_GENERATOR_HPP
