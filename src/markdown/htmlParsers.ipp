#ifndef MARKDOWN_HTML_PARSERS_IPP
#define MARKDOWN_HTML_PARSERS_IPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

//#include <boost/spirit/home/phoenix/container.hpp>

#include "markdown/htmlParsers.hpp"
#include "markdown/stringParsers.hpp"

namespace Markdown {

template <typename Iterator>
HTMLParsers<Iterator>::HTMLParsers(const StringParsers<Iterator>& stringParsers)
:
    HTMLParsers::base_type(blockElement),
    strings(stringParsers)
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    using boost::phoenix::val;
    using boost::phoenix::ref;
    //using boost::phoenix::size;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::alpha;
    using boost::spirit::ascii::alnum;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::digit;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::no_case;    

    //
    // HTML Strings
    //

    HTMLAttribute = //skip(space) [
        *space
        >> +(alnum | '-')
        >> -(
            *space
            >> char_('=')
            >> *space
            >> (strings.quoted | +alnum)
            );

    HTMLTag %= +(alpha | char_("-_:"));

    HTMLComment = string("<!--")
        >> *(char_ - string("-->"))
        >> string("-->");

    //
    // HTML elements
    //

    blockTag %= no_case[
        string("address") | string("blockquote") | string("center") | string("dir") |
        string("div") | string("dl") | string("fieldset") | string("form") |
        lexeme[char_('h') >> char_("1-6")] | string("hr") | string("isindex") | string("menu") |
        string("noframes") | string("noscript") | string("ol") | string("p") |
        string("pre") | string("table") | string("ul") | string("dd") | string("dt") |
        string("frameset") | string("li") | string("tbody") | string("td") |
        string("tfoot") | string("th") | string("thead") | string("tr") | string("script")
        ];

    inlineTag %= HTMLTag - blockTag;

    openBlock = '<'
        >> *space
        >> blockTag [ _val = qi::_1 ]
        >> *space
        >> *HTMLAttribute
        >> *space
        >> '>';

    openInline = '<'
        >> *space
        >> inlineTag [ _val = qi::_1 ]
        >> *space
        >> *HTMLAttribute
        >> *space
        >> '>';

    close = '<'
        >> *space
        >> '/'
        >> *space
        >> no_case[string(_r1)]
        >> *space
        >> '>';

    nonEmptyBlockElement = openBlock [ _a = qi::_1 ]
        >> *( +(char_ - '<') | blockElement | inlineElement | HTMLComment )
        >> close(_a);

    nonEmptyInlineElement = openInline [ _a = qi::_1 ]
        >> *( +(char_ - '<') | inlineElement | HTMLComment )
        >> close(_a);

    emptyBlockElement = '<'
        >> *space
        >> blockTag
        >> *HTMLAttribute
        >> *space
        >> -lit('/')  // Really only optional for "empty" elements like <hr>
        >> *space
        >> '>';

    emptyInlineElement = '<'
        >> *space
        >> inlineTag
        >> *space
        >> *HTMLAttribute
        >> *space
        >> '/'
        >> *space
        >> '>';

    blockElement = nonEmptyBlockElement | emptyBlockElement;

    inlineElement = nonEmptyInlineElement | emptyInlineElement;
}

} // namespace Markdown {

#endif // HTML_PARSERS_IPP
