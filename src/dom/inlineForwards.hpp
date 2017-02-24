#ifndef DOM_INLINE_FORWARDS_HPP
#define DOM_INLINE_FORWARDS_HPP

#include <vector>
#include <iosfwd>

#include <boost/variant.hpp>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/include/vector_fwd.hpp>

namespace DOM {

struct EmphType;
struct StrongType;
struct CodeType;
struct InlineHTMLType;
struct AutoLinkEmailType;
struct AutoLinkURLType;
template <typename ElemType> struct CompoundInline;
template <typename ElemType> struct SimpleInline;

struct LineBreak;

typedef CompoundInline<EmphType> Emph;
typedef CompoundInline<StrongType> Strong;
typedef SimpleInline<InlineHTMLType> InlineHTML;
typedef SimpleInline<CodeType> Code;
typedef SimpleInline<AutoLinkEmailType> AutoLinkEmail;
typedef SimpleInline<AutoLinkURLType> AutoLinkURL;

typedef boost::fusion::vector<
    bool, // img
    std::string, // label
    std::string, // source
    boost::optional<std::string> // title
    > ExplicitLink;

typedef boost::fusion::vector<
    bool, // img
    std::string, // label
    boost::optional<std::string> // ref
    > ReferenceLink;

typedef boost::variant<
    boost::recursive_wrapper<Emph>,
    boost::recursive_wrapper<Strong>,
    InlineHTML,
    Code,
    ExplicitLink,
    ReferenceLink,
    AutoLinkEmail,
    AutoLinkURL,
    LineBreak,
    std::string
    > Inline;

struct Inlines: public std::vector<Inline>, public boost::equality_comparable<Inlines>
{
    Inlines();
    
    template <typename Iter>
    Inlines(Iter start, Iter end) : std::vector<Inline>(start, end) {}

    bool operator == (const Inlines& another) const;
};

std::ostream& operator << (std::ostream&, const Inlines&);

} // namespace DOM {

#endif // INLINE_FORWARDS_HPP
