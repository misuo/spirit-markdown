#ifndef MARKDOWN_BLOCK_PARSER_LIST_IPP
#define MARKDOWN_BLOCK_PARSER_LIST_IPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/phoenix/statement/if.hpp>
#include <boost/phoenix/statement/sequence.hpp>
#include <boost/phoenix/stl/container.hpp>
//#include <boost/spirit/home/phoenix/statement/if.hpp>
//#include <boost/spirit/home/phoenix/statement/sequence.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

#include "markdown/blockParser.hpp"

namespace Markdown {

template <typename Iterator>
void BlockParser<Iterator>::constructListParsers()
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::digit;
    using boost::spirit::ascii::blank;
    using boost::phoenix::ref;
    using boost::phoenix::push_back;
    using boost::phoenix::empty;
    using boost::phoenix::clear;

    //
    // Lists
    //

    static const std::string nl("\n");

    // FIXME: this is mostly a copy'n'paste of subPara
    listItemContents = !strings.blankLine
        >> +(inline_ - eol) [ push_back(_val, _1) ]
        >> eol [ _a = boost::phoenix::ref(nl) ]
        >> *(
            -lit(_r1) 
            >> !(strings.indent | enumerator | bullet | strings.blankLine)
            >> +(inline_ - eol) [
                if_(!empty(_a)) [
                    push_back(_val, _a),
                    clear(_a)
                    ],
                push_back(_val, _1) ]
            >> eol [ _a = boost::phoenix::ref(nl) ] );

    listItemContents.name("listItemContents");
    //debug(listItemContents);

    listItemContinuation %=
        omit[
            *( lit(_r1) >> strings.blankLine )
            >> lit(_r1) [ _a = _r1 ]
            >> strings.indent [ _a += _1 ]
            ]
        >> (unorderedSubList(_a)
            | orderedSubList(_a)
            | subBlockquote(_a)
            | subPara(_a)
            )
        ;

    listItemContinuation.name("listItemContinuation");
    //debug(listItemContinuation);

    bullet = !horizontalRule
        >> strings.nonIndentSpace
        >> char_("+*-")
        >> +blank;

    unorderedListItem %=
        bullet
        >> listItemContents(_r1)
        >> *listItemContinuation(_r1);

    unorderedListItem.name("unorderedListItem");
    //debug(unorderedListItem);

    unorderedSubList %=
        eps [ _a = true ]
        >> unorderedListItem(_r1) % (
            -(strings.indentedBlankLine(_r1)[ _a = false ]) >> lit(_r1))
        >> attr(_a);

    unorderedSubList.name("unorderedSubList");
    //debug(unorderedSubList);

    unorderedList %= unorderedSubList(std::string());

    unorderedList.name("unorderedList");
    //debug(unorderedList);

    enumerator = strings.nonIndentSpace
        >> +digit
        >> char_('.')
        >> +blank;

    orderedListItem %= 
        enumerator
        >> listItemContents(_r1)
        >> *listItemContinuation(_r1);

    orderedListItem.name("orderedListItem");

    orderedSubList %=
        eps [ _a = true ]
        >> orderedListItem(_r1) % (
            -(strings.indentedBlankLine(_r1)[ _a = false ]) >> lit(_r1))
        >> attr(_a);

    orderedSubList.name("orderedSubList");

    orderedList %= orderedSubList(std::string());

    orderedList.name("orderedList");
}

} // namespace Markdown {

#endif // MARKDOWN_BLOCK_PARSER_LIST_IPP
