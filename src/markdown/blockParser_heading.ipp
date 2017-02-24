#ifndef MARKDOWN_HEADING_PARSER_IPP
#define MARKDOWN_HEADING_PARSER_IPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

#include "markdown/blockParser.hpp"

namespace Markdown {

template <typename Iterator>
void BlockParser<Iterator>::constructHeadingParsers()
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    // using boost::phoenix::val;
    // using boost::phoenix::ref;
    using boost::phoenix::size;
    using boost::phoenix::push_back;
    using boost::phoenix::at_c;
    // using boost::phoenix::construct;
    // using boost::phoenix::begin;
    // using boost::phoenix::end;
    // using boost::spirit::ascii::char_;
    // using boost::spirit::ascii::alpha;
    // using boost::spirit::ascii::alnum;
    // using boost::spirit::ascii::digit;
    // using boost::spirit::ascii::string;
    // using boost::spirit::ascii::no_case;    
    // using boost::spirit::ascii::blank;

    //
    // Headings
    // 
    hashes = repeat(1, 6)[ char_('#') ] [ _val = size(_1) ];

    trailingHashes = *blank >> *char_('#') >> eol;

    atxHeading %= hashes
        >> omit[+blank]
        >> +(inline_ - trailingHashes)
        >> trailingHashes;
    
    atxHeading.name("atxHeading");

    setextUnderline = repeat(3, inf)[char_(_r1)] [ _val = _r2 ];

    setextHeading =
        +( inline_ - eol) [ push_back(at_c<1>(_val), _1) ]
        >> eol
        >> (setextUnderline('=', 1) | setextUnderline('-', 2)) [ at_c<0>(_val) = _1 ]
        >> eol;

    heading %= (atxHeading | setextHeading) >> omit[*strings.blankLine];
}

} // namespace Markdown {

#endif // MARKDOWN_HEADING_PARSER_IPP
