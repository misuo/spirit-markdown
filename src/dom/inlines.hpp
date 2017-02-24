#ifndef DOM_INLINES_HPP
#define DOM_INLINES_HPP

#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/not_equal_to.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "dom/inlineForwards.hpp"

namespace DOM {

struct InlinePrinter : public boost::static_visitor<>
{
    InlinePrinter(std::ostream& s)
    : str(s) {}
    
    std::ostream& str;

    template <typename T>
    void operator() (const T& t) const
    {
        str << t;
    }

    void operator() (const std::string& s) const
    {
        str << '\"' << s << '\"';
    }
};

struct EmphType
{
    static const char * desc;
};

struct StrongType
{
    static const char * desc;
};

struct CodeType
{
    static const char * desc;
};

struct InlineHTMLType
{
    static const char * desc;
};

struct AutoLinkEmailType
{
    static const char * desc;
};

struct AutoLinkURLType
{
    static const char * desc;
};


struct LineBreak : public boost::equality_comparable<LineBreak>
{
    bool operator == (const LineBreak& other) const
    { return true; }
};

std::ostream& operator << (std::ostream&, const LineBreak&);


template <typename ElemType>
struct CompoundInline : public std::vector<Inline>
{
    CompoundInline() {}

    explicit CompoundInline(const char * simple) { push_back(std::string(simple)); }

    template <typename Cont>
    CompoundInline(const Cont& c) : std::vector<Inline>(boost::begin(c), boost::end(c)) {}

    bool operator == (const CompoundInline<ElemType>& another) const;
};

template <typename ElemType>
std::ostream& operator << (std::ostream&, const CompoundInline<ElemType>&);

template <typename ElemType>
struct SimpleInline : public std::string
{
    SimpleInline() {}

    template <typename T>
    SimpleInline(T t) : std::string(t) {}

    template <typename Iter>
    SimpleInline(Iter b, Iter e) : std::string(b, e) {}

    // bool operator == (const SimpleInline<ElemType>& another) const
};

template <typename ElemType>
std::ostream& operator << (std::ostream&, const SimpleInline<ElemType>&);

std::ostream& operator << (std::ostream&, const ExplicitLink&);

std::ostream& operator << (std::ostream&, const ReferenceLink&);

std::ostream& operator << (std::ostream&, const std::vector<Inline>&);

} // namespace DOM {

#endif // MARKDOWN_INLINES_HPP
