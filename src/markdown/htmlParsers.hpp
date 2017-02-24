#ifndef MARKDOWN_HTML_PARSERS_HPP
#define MARKDOWN_HTML_PARSERS_HPP

#include <boost/spirit/include/qi.hpp>

#include "markdown/forwards.hpp"

namespace Markdown {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct HTMLParsers : public qi::grammar<Iterator>
{
    HTMLParsers(const StringParsers<Iterator>& stringParsers);

    const StringParsers<Iterator>& strings;

    // HTML Strings
    qi::rule<Iterator> HTMLAttribute;
    qi::rule<Iterator> HTMLComment;
    qi::rule<Iterator, std::string()> HTMLTag;

    // HTML Elements
    qi::rule<Iterator, qi::locals<std::string> > nonEmptyBlockElement, nonEmptyInlineElement;
    qi::rule<Iterator, std::string()> openBlock, openInline, inlineTag, blockTag;
    qi::rule<Iterator, void(std::string)> close;
    qi::rule<Iterator> emptyBlockElement, emptyInlineElement;
    qi::rule<Iterator> blockElement, inlineElement;
};

} // namespace Markdown {

#endif // MARKDOWN_HTML_PARSERS_HPP
