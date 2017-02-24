#include <ostream>
#include <algorithm>

#include <boost/fusion/container.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "dom/inlines.hpp"

namespace DOM {

const char * EmphType::desc = "Emph";
const char * StrongType::desc = "Strong";
const char * CodeType::desc = "Code";
const char * InlineHTMLType::desc = "InlineHTML";
const char * AutoLinkEmailType::desc = "AutoLinkEmail";
const char * AutoLinkURLType::desc = "AutoLinkURL";

std::ostream& operator << (std::ostream& s, const LineBreak&)
{
    s << "LineBreak[]";
    return s;
}

template <typename ElemType>
std::ostream& operator << (std::ostream& s, const SimpleInline<ElemType>& emph)
{
    s << ElemType::desc << '[' << static_cast<std::string>(emph) << ']';
    return s;
}

template <typename ElemType>
std::ostream& operator << (std::ostream& s, const CompoundInline<ElemType>& emph)
{
    s << ElemType::desc << '[';
    InlinePrinter p(s);
    std::for_each(emph.begin(), emph.end(), boost::apply_visitor(p));
    s << ']';
    return s;
}


template <typename ElemType>
bool CompoundInline<ElemType>::operator==(const CompoundInline<ElemType>& another) const
{
    return (&another == this) ||
        (size() == another.size() &&
         std::equal(begin(), end(), another.begin()));
}

std::ostream& operator << (std::ostream& s, const ExplicitLink& el)
{
    using boost::fusion::at_c;

    s << "ExplicitLink[ " << (at_c<0>(el)? "img ":"")
      << "label: \"" << at_c<1>(el) << "\" "
      << "source: \"" << at_c<2>(el) << "\" "
      << "title: \"" << (at_c<3>(el)? *at_c<3>(el) : std::string("none")) << "\" ]";
    return s;
}

std::ostream& operator << (std::ostream& s, const ReferenceLink& rl)
{
    using boost::fusion::at_c;

    s << "ReferenceLink[ " << (at_c<0>(rl)? "img ":"")
      << "label: \"" << at_c<1>(rl) << "\" "
      << "ref: \"" << (at_c<2>(rl)? *at_c<2>(rl) : std::string("none")) << "\" ]";
    return s;
}

std::ostream& operator << (std::ostream& s, const std::vector<Inline>& v)
{
    using boost::phoenix::arg_names::arg1;
    using boost::phoenix::val;

    std::for_each(v.begin(), v.end(), s << val('\"') << arg1 << '\"');
    return s;
}

std::ostream& operator << (std::ostream& s, const Inlines& in)
{
    InlinePrinter p(s);
    std::for_each(in.begin(), in.end(), boost::apply_visitor(p));
    return s;
}

Inlines::Inlines()
{}

bool Inlines::operator== (const Inlines& another) const
{
    return (&another == this)
        || ((size() == another.size())
            && std::equal(begin(), end(), another.begin()));
}


template class CompoundInline<EmphType>;
template class CompoundInline<StrongType>;
template class SimpleInline<InlineHTMLType>;
template class SimpleInline<CodeType>;
template class SimpleInline<AutoLinkEmailType>;
template class SimpleInline<AutoLinkURLType>;

} // namespace DOM {
