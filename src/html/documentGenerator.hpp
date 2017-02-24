#ifndef HTML_DOCUMENT_GENERATOR_HPP
#define HTML_DOCUMENT_GENERATOR_HPP

#include <boost/spirit/include/karma.hpp>

#include "dom/blockForwards.hpp"

#include "html/blockGenerator.hpp"

namespace HTML {

namespace karma = boost::spirit::karma;

template <typename Iterator>
struct DocumentGenerator : public karma::grammar<Iterator, DOM::Document()>
{
    DocumentGenerator(const DOM::ReferenceMap& refmap_);

    BlockGenerator<Iterator> block;

    karma::rule<Iterator, DOM::Document()> document;
};
        
} // namespace HTML {

#endif // DOCUMENT_HTML_GENERATOR_HPP
