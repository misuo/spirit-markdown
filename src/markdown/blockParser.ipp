#ifndef MARKDOWN_BLOCK_PARSER_IPP
#define MARKDOWN_BLOCK_PARSER_IPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/phoenix/statement/if.hpp>
#include <boost/phoenix/statement/sequence.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/core/reference.hpp>   // For access to ref()
#include <boost/phoenix/stl/container.hpp>    // For access to lazy container empty() and clear().

//#include <boost/spirit/home/phoenix/statement/if.hpp>
//#include <boost/spirit/home/phoenix/statement/sequence.hpp>
//#include <boost/spirit/home/phoenix/object/construct.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

#include "markdown/blockParser.hpp"

namespace Markdown {

template <typename Iterator>
BlockParser<Iterator>::BlockParser()
:
    BlockParser::base_type(block),
    strings(),
    html(strings),
    inline_(strings, html)
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    using boost::phoenix::val;
    using boost::phoenix::ref;
    using boost::phoenix::size;
    using boost::phoenix::push_back;
    using boost::phoenix::at_c;
    using boost::phoenix::construct;
    using boost::phoenix::begin;
    using boost::phoenix::end;
    using boost::phoenix::empty;
    using boost::phoenix::clear;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::alpha;
    using boost::spirit::ascii::alnum;
    using boost::spirit::ascii::digit;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::no_case;    
    using boost::spirit::ascii::blank;

    //
    // Blocks
    //

    // see description in header file as to why we do this...
    constructHeadingParsers();
    constructListParsers();

    static const std::string nl("\n");

    // each of the subBlock rules assumes that there is a container prefix which has been read from
    // input, but is also passed in as _r1 so that it can be checked at the start of successive
    // lines. That's why each of the subBlock parsers is a bit long-winded, it has to recognise the
    // prefix only on successive lines...

    subPara = !strings.blankLine
        >> +(inline_ - eol) [ push_back(_val, _1) ]
        >> eol [ _a = boost::phoenix::ref(nl) ]
        >> *(
            -lit(_r1) 
            >> !(bullet | enumerator | strings.blankLine)
            >> +(inline_ - eol) [
                if_(!empty(_a)) [
                    push_back(_val, _a),
                    clear(_a)
                    ],
                push_back(_val, _1) ]
            >> eol [ _a = boost::phoenix::ref(nl) ] );

    subPara.name("subPara");
    //debug(subPara);
    para %= subPara(std::string());

    subBlockquote =
        raw[ strings.nonIndentSpace
             >> char_('>')
             >> -(strings.nonIndentSpace - strings.indent) ] [ _a = _r1 + construct<std::string>(begin(_1), end(_1)) ]
        >> (
            subVerbatim(_a)
            | subBlockquote(_a)
            | orderedSubList(_a)
            | unorderedSubList(_a)
            | subPara(_a)
            ) [ push_back(_val, _1) ]
        >> *(
            -strings.blankLine
            >> lit(_a)
            >> (strings.blankLine
                | ( subVerbatim(_a)
                    | subBlockquote(_a)
                    | orderedSubList(_a)
                    | unorderedSubList(_a)
                    | subPara(_a)
                    ) [ push_back(_val, _1) ]
                )
            )
        ;

    subBlockquote.name("subBlockquote");
    //debug(subBlockquote);
    blockquote %= subBlockquote(std::string());

    subVerbatim = 
        !strings.blankLine
        >> strings.indent
        >> raw[
            +(char_ - (eol|eoi)) >> -eol
            ] [ push_back(_val, construct<std::string>(begin(_1), end(_1))) ]
        >> *(
            -( strings.blankLine [ _a = boost::phoenix::ref(nl) ] )
            >> lit(_r1)
            >> strings.indent
            >> raw[
                +(char_ - (eol|eoi)) >> -eol
                ] [
                    if_(!empty(_a)) [
                        push_back(_val, _a),
                        clear(_a)
                        ],
                    push_back(_val, construct<std::string>(begin(_1), end(_1)))
                    ]
            )
        ;

    subVerbatim.name("subVerbatim");
    //debug(subVerbatim);
    verbatim %= subVerbatim(std::string());

    horizontalRule = ( strings.nonIndentSpace
                       >> ( ( char_('*') >> repeat(2, inf)[ *blank >> char_('*')] )
                            | ( char_('-') >> repeat(2, inf)[ *blank >> char_('-')] )
                            | ( char_('_') >> repeat(2, inf)[ *blank >> char_('_')] ) )
                       >> *blank
                       >> eol
                       >> +strings.blankLine ) [ _val = construct<DOM::HorizontalRule>() ];
    
    //plain = +inline_ >> -blankLine;

    htmlBlock %= raw[html.blockElement];

    block %= blockquote | verbatim | htmlBlock | heading | orderedList | unorderedList | horizontalRule | para;
}

} // namespace Markdown {

#endif // MARKDOWN_BLOCK_PARSER_IPP
