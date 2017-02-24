#ifndef HTML_BLOCK_GENERATOR_IPP
#define HTML_BLOCK_GENERATOR_IPP

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include "html/blockGenerator.hpp"

namespace HTML { 

template <typename Iterator>
BlockGenerator<Iterator>::BlockGenerator(const DOM::ReferenceMap& refmap_)
    : BlockGenerator::base_type(block)
    , inline_(refmap_)
{
    using boost::spirit::_1;
    using boost::spirit::_val;
    using boost::spirit::eol;
    using boost::spirit::int_;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::string;
    using boost::phoenix::at_c;

    para %= "<p>"
        << *inline_
        << "</p>"
        << eol;

    blockquote %= "<blockquote>"
        << eol
        << *block
        << "</blockquote>"
        << eol;

    headingReordered %= "<h"
        << int_
        << ">"
        << *inline_
        << "</h"
        << int_
        << ">"
        << eol;

    heading %= headingReordered;

    verbatim %= "<pre><code>"
        << *inline_.text
        << "</code></pre>"
        << eol;

    horizontalRule = "<hr />"
        << eol;

    listItem %= "<li>"
        // TODO: handle tight flag in _r1
        << *inline_
        << *block // FIXME
        << "</li>"
        << eol;

    unorderedList = "<ul>"
        << eol
        << ( *listItem(at_c<1>(_val)) ) [ _1 = at_c<0>(_val) ]
        << "</ul>"
        << eol;

    orderedList = "<ol>"
        << eol
        << ( *listItem(at_c<1>(_val)) ) [ _1 = at_c<0>(_val) ]
        << "</ol>"
        << eol;
        
    htmlBlock %= *char_
        << eol;

    block = para | blockquote | heading | verbatim | horizontalRule | unorderedList | orderedList | htmlBlock;
}

}  // namespace HTML { 

#endif // HTML_BLOCK_GENERATOR_IPP
