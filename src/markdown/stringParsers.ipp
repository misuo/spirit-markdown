#pragma once

#include <boost/phoenix/stl/container.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "markdown/stringParsers.hpp"

namespace Markdown
{

template <typename Iterator>
StringParsers<Iterator>::StringParsers() : StringParsers<Iterator>::base_type(blankLine)
{
    //using namespace boost::spirit;
    using namespace boost::spirit::qi;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::alnum;
    using boost::spirit::ascii::digit;
    using boost::spirit::ascii::xdigit;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::blank;
    //using boost::phoenix::push_back;

    //
    // Characters and tokens
    //
    spnl = *blank >> -eol >> *blank;

    specialChar %= char_("*_`&[]<!\\"); // | extendedSpecialChar;
    escapedChar %= lit('\\') >> char_;
    normalChar %= char_ - (specialChar | space);

    special %= repeat(1)[specialChar];

    //
    // entities
    //

    predefinedEntities.add
        ( "&amp;", '&' )
        ( "&lt;",  '<' );

    numericEntity %= char_('&')
        >>  char_('#')
        >> (
            (char_("xX") >> repeat(1,4)[xdigit]) // hex
            | repeat(1,8)[digit]  // decimal
            )
        >> char_(';');

    namedEntity %= char_('&')
        >> (char_ - '#')
        >> *alnum
        >> char_(';');

    entity %= repeat(1)[predefinedEntities] | namedEntity | numericEntity;

    //
    // Strings
    //
    nonIndentSpace %= repeat(0, 3)[char_(' ')];
    indent %= string("\t") | repeat(4)[char_(' ')];

    blankLine = *blank >> eol;
    blankLine.name("blankLine");
    //debug(blankLine);

    indentedBlankLine = lit(_r1) >> blankLine;

    lineBreak = string("  ") >> *blank >> &eol;
    
    str %= +(normalChar | escapedChar);
    spaces %= +blank;

    labelContents = *( +(char_ - char_("[]")) | label );
        
    label %= lit('[') >> raw[labelContents] >> lit(']');

    sourceContents = *(
        +( char_ - (space | char_("()<>")) )
        | (char_('(') >> sourceContents >> char_(')') )
        | ( char_('<') >> sourceContents >> char_('>') )
        );

    source %= 
        (lit('<') >> raw[sourceContents] >> lit('>'))
        | raw[sourceContents];

    source.name("source");

    endTitle = -lit(_r1) >> &(*blank >> (char_(')') | eol | eoi));

    title %= omit[ char_("\'\"") [ _a = _1 ] ]
        >> *( char_ - endTitle(_a))
        >> endTitle(_a);

    title.name("title");

    quoted %= lexeme[
        char_("\'\"") [ _a = _1 ]
        >> *(char_ - char_(_a))
        >> char_(_a)
    ];

    quoted.name("quoted");
}

}