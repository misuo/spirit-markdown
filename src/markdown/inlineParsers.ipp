#ifndef MARKDOWN_INLINE_PARSERS_IPP
#define MARKDOWN_INLINE_PARSERS_IPP

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>
#include <boost/phoenix/stl/container.hpp>

#include "dom/inlines.hpp"

#include "markdown/htmlParsers.hpp"
#include "markdown/inlineParsers.hpp"
#include "markdown/stringParsers.hpp"

namespace Markdown {

template <typename Iterator>
InlineParsers<Iterator>::InlineParsers( const StringParsers<Iterator>& stringParsers, const HTMLParsers<Iterator>& htmlParsers ) :
    InlineParsers::base_type(inline_),
    strings(stringParsers),
    html(htmlParsers)
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    // using boost::phoenix::val;
    // using boost::phoenix::ref;
    // using boost::phoenix::push_back;
    // using boost::phoenix::copy;
    using boost::phoenix::size;
    using boost::phoenix::construct;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::alpha;
    using boost::spirit::ascii::alnum;
    using boost::spirit::ascii::digit;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::no_case;    

    starLine = repeat(4, inf)[char_('*')] | ( blank >> +char_('*') >> &blank );
    ulLine %= repeat(4, inf)[char_('_')] | ( blank >> +char_('_') >> &blank );

    lineBreak = strings.lineBreak [ _val = construct<DOM::LineBreak>() ];

    //
    // Emph and Strong
    //
    oneCharOpen = !starLine >> lit(_r1) >> !space;
    oneCharClose = !space >> !twoCharOpen(_r1) >> lit(_r1);

    emphWith %=
        oneCharOpen(_r1)
        >> *(inline_ - oneCharClose(_r1))
        >> oneCharClose(_r1);

    twoCharOpen = !starLine >> repeat(2)[lit(_r1)] >> !space;
    twoCharClose = !space >> repeat(2)[lit(_r1)];

    strongWith %=
        twoCharOpen(_r1)
        >> *(inline_ - twoCharClose(_r1))
        >> twoCharClose(_r1);

    emph %= emphWith('*') | emphWith('_');
    strong %= strongWith('*') | strongWith('_');

    emph.name("emph");
    strong.name("strong");

    //
    // Code
    //
    ticks = repeat(_r1)[lit('`')];

    code %= 
        omit[
            (+char_('`')) [ _a = size(qi::_1) ]
            ]
        >>  raw[
            *(
                +(char_ - (ticks(_a) | '\"' | '\''))
                | strings.quoted
                )
            ]
        >> ticks(_a);

    code.name("code");
    //debug(code);
    
    //
    // Links
    //

    explicitLink %= 
        matches[char_('!')]
        >> strings.label
        >> strings.spnl
        >> lit('(')
        >> omit[*blank]
        >> strings.source
        >> strings.spnl
        >> -strings.title
        >> omit[*blank]
        >> lit(')');

    referenceLink %= 
        matches[char_('!')]
        >> strings.label
        >> strings.spnl
        >> ((strings.label - lit("[]")) || -lit("[]"));
    
    autolinkUrl %= lit('<')
        >> +alpha
        >> char_(':')
        >> char_('/')
        >> char_('/')
        >> +(char_ - char_("\n>"))
        >> lit('>');

    autolinkEmail %= lit('<')
        >> +alpha
        >> char_('@')
        >> +(char_ - char_("\n>"))
        >> lit('>');

    explicitLink.name("explicitLink");
    referenceLink.name("referenceLink");
    autolinkUrl.name("autolinkUrl");
    autolinkEmail.name("autolinkEmail");


    // Inline HTML

    inlineHTML %= raw[
        html.inlineElement | html.HTMLComment
        ];

    inlineHTML.name("inlineHTML");

    //
    // All inlines
    //
    
    inline_ %= strong | emph | strings.str | strings.entity | lineBreak | strings.spaces | inlineHTML | code |
        explicitLink | referenceLink | autolinkUrl | autolinkEmail | strings.special;

    inline_.name("inline");

    inlines %= +inline_;
}

} // namespace Markdown {

#endif // INLINES_GRAMMAR_IPP
