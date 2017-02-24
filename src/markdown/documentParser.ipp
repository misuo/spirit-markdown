#ifndef MARKDOWN_DOCUMENT_PARSER_IPP
#define MARKDOWN_DOCUMENT_PARSER_IPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/stl/container.hpp>
//#include <boost/spirit/home/phoenix/object/construct.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

#include "markdown/documentParser.hpp"

namespace Markdown {

namespace phx = boost::phoenix;

template <typename Iterator>
DocumentParser<Iterator>::DocumentParser(DOM::ReferenceMap& refmap_)
    : DocumentParser::base_type(document)
    , refmap(refmap_)
{
    using namespace boost::spirit::qi;
    using boost::phoenix::at_c;
    using boost::phoenix::push_back;
    using boost::phoenix::insert;
    using boost::phoenix::ref;

    reference = strings.nonIndentSpace
        >> (strings.label - lit("[]"))  [ at_c<1>(_val) = _1 ]
        >> lit(':')
        >> *blank
        >> strings.source       [ at_c<2>(_val) = _1 ]
        >> *blank
        >> -(
            -(eol >> *blank)
            >> strings.title    [ at_c<3>(_val) = _1 ]
            )
        >> *blank
        >> eol
        >> *strings.blankLine;

    reference.name("reference");
    //debug(reference);

    document = *(
        *strings.blankLine
        >> (
            reference [ insert(phx::ref(refmap), _1) ]
            |
            block [ push_back(_val, _1) ]
            )
        )
        >> *strings.blankLine
        >> eoi;

    document.name("document");
    //debug(document);
}

} // namespace Markdown {

#endif // MARKDOWN_DOCUMENT_PARSER_IPP
