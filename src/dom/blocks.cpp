#include <algorithm>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "dom/blocks.hpp"
#include "dom/inlines.hpp"

namespace DOM {

const char * OrderedListType::desc = "OrderedList";
const char * UnorderedListType::desc = "UnorderedList";
const char * BlockquoteType::desc = "Blockquote";

std::ostream& operator << (std::ostream& str, const HorizontalRule&)
{
    str << "HorizontalRule[]";
    return str;
}

Heading::Heading()
 : level(0)
{
}

Heading::Heading(unsigned level_, const char * contents_)
: level(level_)
, contents()
{
    contents.push_back(std::string(contents_));
}

bool Heading::operator == (const Heading& other) const
{
    return level == other.level && contents.size() == other.contents.size()
        && std::equal(contents.begin(), contents.end(), other.contents.begin());
}

std::ostream& operator << (std::ostream& s, const Heading& heading)
{
    s << "Heading[ (" << heading.level << ") ";
    InlinePrinter p(s);
    std::for_each(heading.contents.begin(), heading.contents.end(), boost::apply_visitor(p));
    s << " ]";
    return s;
}


Para::Para()
{
}

Para::Para(const char * simple)
{
    push_back(std::string(simple));
}

bool Para::operator == (const Para& other) const
{
    return size() == other.size()
        && std::equal(begin(), end(), other.begin());
}

std::ostream& operator << (std::ostream& s, const Para& b)
{
    using boost::phoenix::arg_names::arg1;
    using boost::phoenix::val;

    s << "Para[ ";
    std::for_each(b.begin(), b.end(), s << val('\"') << arg1 << '\"');
    s << " ]";
    return s;
}


Verbatim::Verbatim(const char * simple)
{
    push_back(std::string(simple));
}

bool Verbatim::operator == (const Verbatim& other) const
{
    return size() == other.size()
        && std::equal(begin(), end(), other.begin());
}

std::ostream& operator << (std::ostream& s, const Verbatim& b)
{
    using boost::phoenix::arg_names::arg1;
    using boost::phoenix::val;

    s << "Verbatim[ ";
    std::for_each(b.begin(), b.end(), s << val('\"') << arg1 << '\"');
    s << " ]";
    return s;
}


HTMLBlock::HTMLBlock(const char * simple)
:
    std::string(simple)
{
}

bool HTMLBlock::operator == (const HTMLBlock& other) const
{
    return size() == other.size()
        && std::equal(begin(), end(), other.begin());
}

std::ostream& operator << (std::ostream& s, const HTMLBlock& b)
{
    s << "HTMLBlock[ ";
    s << static_cast<const std::string&>(b);
    s << " ]";
    return s;
}


template <typename CompoundType>
CompoundBlock<CompoundType>::CompoundBlock(const char * simple)
{
    push_back(Para(simple));
}

template <typename CompoundType>
bool CompoundBlock<CompoundType>::operator == (const CompoundBlock<CompoundType>& other) const
{
    return size() == other.size()
        && std::equal(begin(), end(), other.begin());
}

template <typename CompoundType>
std::ostream& operator << (std::ostream& s, const CompoundBlock<CompoundType>& b)
{
    s << CompoundType::desc << "[ ";
    InlinePrinter p(s); // works for blocks too
    std::for_each(b.begin(), b.end(), boost::apply_visitor(p));
    s << " ]";
    return s;
}


ListItem::ListItem()
{
}

ListItem::ListItem(const char * simple)
{
    contents.push_back(std::string(simple));
}

bool ListItem::operator == (const ListItem& other) const
{
    return contents.size() == other.contents.size()
        && continuation.size() == other.continuation.size()
        && std::equal(contents.begin(), contents.end(), other.contents.begin())
        && std::equal(continuation.begin(), continuation.end(), other.continuation.begin());
}

std::ostream& operator << (std::ostream& s, const ListItem& item)
{
    s << "ListItem[ ";
    InlinePrinter p(s);
    std::for_each(item.contents.begin(), item.contents.end(), boost::apply_visitor(p));
    if (!item.continuation.empty())
    {
        s << ", ";
        InlinePrinter b(s); // works for blocks too
        std::for_each(item.continuation.begin(), item.continuation.end(), boost::apply_visitor(b));
    }
    s << " ]";
    return s;
}


template <typename ListType>
ListItemContainer<ListType>::ListItemContainer(const char * simple, bool t)
    : tight(t)
{
    items.push_back(ListItem(simple));
}

template <typename ListType>
bool ListItemContainer<ListType>::operator == (const ListItemContainer<ListType>& other) const
{
    return tight == other.tight
        && items.size() == other.items.size()
        && std::equal(items.begin(), items.end(), other.items.begin());
}

template <typename ListType>
std::ostream& operator << (std::ostream& s, const ListItemContainer<ListType>& list)
{
    s << ListType::desc << "[ ";
    s << (list.tight? "tight " : "loose ");
    std::for_each(list.items.begin(), list.items.end(), InlinePrinter(s));
    s << " ]";
    return s;
}

std::ostream& operator << (std::ostream& s, const Document& d)
{
    InlinePrinter p(s);
    std::for_each(d.begin(), d.end(), boost::apply_visitor(p));
    return s;
}

template struct ListItemContainer<OrderedListType>;
template struct ListItemContainer<UnorderedListType>;
template struct CompoundBlock<BlockquoteType>;

} // namespace DOM {
