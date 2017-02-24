#ifndef DOM_BLOCK_FORWARDS_HPP
#define DOM_BLOCK_FORWARDS_HPP

#include <boost/variant.hpp>

#include "dom/inlineForwards.hpp"

namespace DOM {

struct Heading;

// Bit of a hack, we reuse the ExplicitLink inline type for references, just ignore the unneeded img flag.
typedef ExplicitLink Reference;

struct HorizontalRule;

struct Para;
struct Verbatim;
struct HTMLBlock;

template <typename CompoundType> struct CompoundBlock;
struct BlockquoteType;
typedef CompoundBlock<BlockquoteType> Blockquote;

struct UnorderedListType;
struct OrderedListType;
template <typename ListType> struct ListItemContainer;
typedef ListItemContainer<UnorderedListType> UnorderedList;
typedef ListItemContainer<OrderedListType> OrderedList;

struct ListItem;

typedef boost::variant<
    Para,
    Heading,
    Reference,
    Verbatim,
    HTMLBlock,
    HorizontalRule,
    boost::recursive_wrapper<Blockquote>,
    boost::recursive_wrapper<UnorderedList>,
    boost::recursive_wrapper<OrderedList>
    > Block;

typedef std::vector<Block> Document;

} // namespace DOM {

#endif // BLOCK_FORWARDS_HPP
