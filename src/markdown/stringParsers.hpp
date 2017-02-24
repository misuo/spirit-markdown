#ifndef MARKDOWN_STRING_PARSERS_HPP
#define MARKDOWN_STRING_PARSERS_HPP

#include <boost/spirit/include/qi.hpp>

namespace Markdown {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct StringParsers : public qi::grammar<Iterator>
{
    StringParsers();

    // Characters and Tokens
    qi::rule<Iterator, char()> normalChar, escapedChar, specialChar;
    qi::rule<Iterator> spnl;

    // Entities
    qi::symbols<char, char> predefinedEntities;
    qi::rule<Iterator, std::string()> namedEntity, numericEntity, entity;

    // Strings
    qi::rule<Iterator, std::string()> nonIndentSpace;
    qi::rule<Iterator, std::string()> indent;
    qi::rule<Iterator> blankLine, lineBreak;
    qi::rule<Iterator, void(std::string)> indentedBlankLine;

    qi::rule<Iterator, std::string(), qi::locals<char> > quoted;

    // labels, titles and sources (used for links)
    qi::rule<Iterator, void(char)> endTitle;
    qi::rule<Iterator, std::string()> labelContents, sourceContents;
    qi::rule<Iterator, std::string(), qi::locals<char> > title;    
    qi::rule<Iterator, std::string()> label, source;

    qi::rule<Iterator, std::string()> str, spaces, special;
};

} // namespace Markdown {

#endif // MARKDOWN_STRING_PARSERS_HPP
