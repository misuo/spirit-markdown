#ifndef HTML_INLINE_GENERATOR_IPP
#define HTML_INLINE_GENERATOR_IPP

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

//#include <boost/spirit/home/phoenix/object/construct.hpp>

#include "html/inlineGenerator.hpp"

namespace HTML { 

template <typename Iterator>
InlineGenerator<Iterator>::InlineGenerator(const DOM::ReferenceMap& refmap_)
    : InlineGenerator::base_type(inline_)
    , refmap(refmap_)
    , HasRef(refmap_)
    , SourceFor(refmap_)
    , TitleFor(refmap_)
{
    using namespace boost::spirit;
    using boost::spirit::_1;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::char_;
    using boost::phoenix::at_c;
    //using boost::phoenix::construct;

    normalEscapes.add
        ('<', "&lt;")
        ('&', "&amp;")
        ('>', "&gt;")
        ;

    text = *(normalEscapes | char_);

    quotedEscapes.add
        ('\"', "&quot;")
        ;

    quotedText = *(normalEscapes | quotedEscapes | char_);

    emph %= "<em>" << *inline_ << "</em>";
    strong %= "<strong>" << *inline_ << "</strong>";
    inlineHTML %= *char_;
    code %= "<code>" << text << "</code>";

    linkOpen =
        &bool_(true) << "<img src=\""
        | "<a href=\"";
    linkClose =
        &bool_(true) << "</img>"
        | "</a>";

    explicitLinkReordered %= linkOpen
        << string
        << '\"'
        << -(
            " title=\"" << quotedText << '\"'
            )
        << '>'
        << text
        << linkClose
        ;

    explicitLink = explicitLinkReordered;
    //explicitLink.name("explicitLink");

    // TODO: fix this so there's only one map lookup instead of two... Prefer to reuse the
    // explicitLink generator too?
    referenceLinkReordered %= linkOpen
        << string [ _1 = SourceFor(_val) ]
        << '\"'
        << ( -(
                 " title=\"" << quotedText << '\"'
                 ))  [ _1 = TitleFor(_val) ]
        << '>'
        << text
        << linkClose
        ;

    referenceLinkRaw %=
        ( bool_(true) << '!' | eps )
        << '['
        << text
        << ']'
        << -(
            '['
            << text
            << ']'
            );

    referenceLink %= referenceLinkReordered | "###blah###"
        ;
    //referenceLink.name("referenceLink");

    autoLinkEmail =
        "<a href=\"mailto:"
        << text [ _1 = _val ]
        << "\">"
        << text [ _1 = _val ]
        << "</a>";

    autoLinkURL =
        "<a href=\""
        << text [ _1 = _val ]
        << "\">"
        << text [ _1 = _val ]
        << "</a>";

    inline_ %= emph | strong | inlineHTML | code | explicitLink | referenceLink | autoLinkEmail | autoLinkURL | text;
}

}  // namespace HTML { 

#endif // HTML_INLINE_GENERATOR_IPP
