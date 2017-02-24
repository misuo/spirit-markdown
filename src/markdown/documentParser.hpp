#ifndef MARKDOWN_DOCUMENT_PARSER_HPP
#define MARKDOWN_DOCUMENT_PARSER_HPP

#include <boost/spirit/include/qi.hpp>

#include "dom/blockForwards.hpp"
#include "dom/referenceMap.hpp"

#include "markdown/blockParser.hpp"
#include "markdown/stringParsers.hpp"

namespace Markdown {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct DocumentParser : public qi::grammar<Iterator, DOM::Document()>
{
    DocumentParser(DOM::ReferenceMap& refmap_);

    StringParsers<Iterator> strings;
    BlockParser<Iterator> block;
    DOM::ReferenceMap& refmap;

    // Reference. Maybe this doesn't belong here - maybe it's logically a block-level
    // element. However, it only seems useful in a document context. And also if we view this as
    // distinct from a regular block, then we don't need to plumb the ReferenceMap down into the
    // block parser.
    qi::rule<Iterator, DOM::Reference()> reference;

    // Document
    qi::rule<Iterator, DOM::Document()> document;
};

} // namespace Markdown {

#endif // MARKDOWN_DOCUMENT_PARSER_HPP
