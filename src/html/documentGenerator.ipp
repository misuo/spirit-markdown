#ifndef HTML_DOCUMENT_GENERATOR_IPP
#define HTML_DOCUMENT_GENERATOR_IPP

#include "dom/blocks.hpp"

#include "html/documentGenerator.hpp"

namespace HTML { 

template <typename Iterator>
DocumentGenerator<Iterator>::DocumentGenerator(const DOM::ReferenceMap& refmap_)
    : DocumentGenerator::base_type(document)
    , block(refmap_)
{
    using boost::spirit::eol;
    
    document %= block % eol;
}

}  // namespace Html { 

#endif // HTML_DOCUMENT_GENERATOR_IPP
