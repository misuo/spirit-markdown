#ifndef MARKDOWN_INLINE_PARSERS_HPP
#define MARKDOWN_INLINE_PARSERS_HPP

#include <boost/spirit/include/qi.hpp>

#include "dom/inlineForwards.hpp"

#include "markdown/forwards.hpp"

namespace Markdown {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct InlineParsers : public qi::grammar<Iterator, DOM::Inline()>
{
    InlineParsers(
        const StringParsers<Iterator>& stringParsers,
        const HTMLParsers<Iterator>& htmlParsers);

    const StringParsers<Iterator>& strings;
    const HTMLParsers<Iterator>& html;

    // Lines
    qi::rule<Iterator> starLine, ulLine;
    
    qi::rule<Iterator, DOM::LineBreak()> lineBreak;

    // Emph and Strong

    qi::rule<Iterator, void(char)> oneCharOpen;
    qi::rule<Iterator, void(char)> oneCharClose;
    qi::rule<Iterator, void(char)> twoCharOpen;
    qi::rule<Iterator, void(char)> twoCharClose;
    qi::rule<Iterator, DOM::Emph(char)> emphWith;
    qi::rule<Iterator, DOM::Emph()> emph;
    qi::rule<Iterator, DOM::Strong(char)> strongWith;
    qi::rule<Iterator, DOM::Strong()> strong;

    // Code

    qi::rule<Iterator, void(size_t)> ticks;
    qi::rule<Iterator, DOM::Code(), qi::locals<size_t> > code;

    // Links

    qi::rule<Iterator, DOM::ExplicitLink()> explicitLink;
    qi::rule<Iterator, DOM::ReferenceLink()> referenceLink;
    qi::rule<Iterator, DOM::AutoLinkURL()> autolinkUrl;
    qi::rule<Iterator, DOM::AutoLinkEmail()> autolinkEmail;

    // HTML
    qi::rule<Iterator, DOM::InlineHTML()> inlineHTML;

    // Inline

    qi::rule<Iterator, DOM::Inline()> inline_;
    qi::rule<Iterator, DOM::Inlines()> inlines;
};

} // namespace Markdown {

#endif // INLINES_PARSERS_HPP
