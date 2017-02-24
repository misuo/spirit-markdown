#ifndef DOM_BLOCKS_HPP
#define DOM_BLOCKS_HPP

#include <boost/operators.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "dom/blockForwards.hpp"
#include "dom/inlines.hpp"

namespace DOM {

struct HorizontalRule : public boost::equality_comparable<HorizontalRule>
{
    bool operator == (const HorizontalRule& other) const
    { return true; }
};

std::ostream& operator << (std::ostream&, const HorizontalRule&);


struct Heading : public boost::equality_comparable<Heading>
{
    unsigned level;
    std::vector<Inline> contents;

    Heading();

    // Convenience constructor - simple string
    Heading(unsigned level_, const char * contents_);

    // Convenience constructor - list of inlines
    template <typename Cont>
    Heading(unsigned level_, const Cont& c)
    : level(level_), contents(boost::begin(c), boost::end(c)) {}

    bool operator == (const Heading& other) const;
};

std::ostream& operator << (std::ostream&, const Heading&);

struct Para
: public std::vector<Inline>
, boost::equality_comparable<Para>
{
    Para();

    // Convenience constructor - simple string
    Para(const char *simple);

    // Convenience constructor - list of inlines
    template <typename Cont>
    Para(const Cont& c)
    : std::vector<Inline>(boost::begin(c), boost::end(c)) {}

    bool operator == (const Para& another) const;
};

std::ostream& operator << (std::ostream&, const Para&);

struct Verbatim : public std::vector<std::string>, boost::equality_comparable<Verbatim>
{
    Verbatim() {}
    
    // Convenience constructor - single line
    Verbatim(const char * simple);

    // Convenience constructor - list of inlines
    template <typename Cont>
    Verbatim(const Cont& c) : std::vector<std::string>(boost::begin(c), boost::end(c)) {}

    bool operator == (const Verbatim& another) const;
};

std::ostream& operator << (std::ostream&, const Verbatim&);


struct HTMLBlock
: public std::string
, boost::equality_comparable<HTMLBlock>
{
    HTMLBlock() {}
    
    // construct from raw
    template <typename Iter>
    HTMLBlock(const Iter& b, const Iter& e)
    : std::string(b, e) {}

    // Convenience constructor - single line
    HTMLBlock(const char * simple);

    bool operator == (const HTMLBlock& another) const;
};

std::ostream& operator << (std::ostream&, const HTMLBlock&);



template <typename CompoundType>
struct CompoundBlock
: public std::vector<Block>
, boost::equality_comparable<CompoundBlock<CompoundType> >
{
    CompoundBlock() {}

    explicit CompoundBlock(const char * simple);

    template <typename Cont>
    CompoundBlock(const Cont& c)
    : std::vector<Block>(boost::begin(c), boost::end(c)) {}

    bool operator == (const CompoundBlock<CompoundType>& another) const;
};

template <typename CompoundType>
std::ostream& operator << (std::ostream&, const CompoundBlock<CompoundType>&);

struct BlockquoteType {
    static const char * desc;
};



struct ListItem : public boost::equality_comparable<ListItem>
{
    std::vector<Inline> contents;
    std::vector<Block> continuation;

    ListItem();

    ListItem(const char * contents_);
    
    template <typename Cont>
    ListItem(const Cont& c)
        : contents(boost::begin(c), boost::end(c))
    {}

    template <typename Cont1, typename Cont2>
    ListItem(const Cont1& c1, const Cont2& c2)
        : contents(boost::begin(c1), boost::end(c1))
        , continuation(boost::begin(c2), boost::end(c2))
    {}

    bool operator == (const ListItem& other) const;
};

std::ostream& operator << (std::ostream&, const ListItem&);


struct UnorderedListType {
    static const char * desc;
};
struct OrderedListType {
    static const char * desc;
};

template <typename ListType>
struct ListItemContainer
: public boost::equality_comparable< ListItemContainer<ListType> >
{
    std::vector<ListItem> items;
    bool tight;

    ListItemContainer() : tight(true) {}

    // convenience constructor - single list item with simple contents
    explicit ListItemContainer(const char * simple, bool tight = true);

    template <typename Cont>
    ListItemContainer(const Cont& c, bool t = true)
    : items(boost::begin(c), boost::end(c)), tight(t) {}

    bool operator == (const ListItemContainer<ListType>& another) const;
};

template <typename ListType>
std::ostream& operator << (std::ostream&, const ListItemContainer<ListType>&);

std::ostream& operator << (std::ostream&, const Document&);
    
} // namespace DOM {


// Must be in global namespace

BOOST_FUSION_ADAPT_STRUCT(
    DOM::ListItem,
    (std::vector<DOM::Inline>, contents)
    (std::vector<DOM::Block>, continuation)
    )

BOOST_FUSION_ADAPT_STRUCT(
    DOM::OrderedList,
    (std::vector<DOM::ListItem>, items)
    (bool, tight)
    )

BOOST_FUSION_ADAPT_STRUCT(
    DOM::UnorderedList,
    (std::vector<DOM::ListItem>, items)
    (bool, tight)
    )

BOOST_FUSION_ADAPT_STRUCT(
    DOM::Heading,
    (unsigned, level)
    (std::vector<DOM::Inline>, contents)
    )


#endif // BLOCKS_HPP
