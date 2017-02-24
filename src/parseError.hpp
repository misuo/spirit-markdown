#ifndef PARSE_ERROR_HPP
#define PARSE_ERROR_HPP

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

template <typename Iterator>
class ParseError : public std::runtime_error
{
public:
    ParseError(Iterator start, Iterator end, Iterator where, const char * what);

private:
    static std::string ConstructMessage(Iterator start, Iterator end, Iterator where, const char * what);
};

template <typename Iterator>
ParseError<Iterator>::ParseError(Iterator start, Iterator end, Iterator where, const char * what)
:
    std::runtime_error(ConstructMessage(start, end, where, what))
{
}

template <typename Iterator>
std::string ParseError<Iterator>::ConstructMessage(Iterator start, Iterator end, Iterator where, const char * what)
{
    typedef std::reverse_iterator<Iterator> ReverseIterator;

    std::ostringstream s;
    Iterator sol = std::find(ReverseIterator(where), ReverseIterator(start), '\n').base();
    if (*sol == '\n')
        ++sol;
    Iterator eol = std::find(where, end, '\n');
    size_t lineNum = std::count(start, sol, '\n') + 1;
    size_t colNum = std::distance(sol, where);

    s << what << ": line " << lineNum << ", column " << colNum << '\n';
                
    std::copy(sol, eol, std::ostream_iterator<char>(s));
    s << '\n';
    
    std::fill_n(std::ostream_iterator<char>(s), colNum, ' ');
    s << "^\n";

    return s.str();
}

#endif // PARSE_ERROR_HPP
