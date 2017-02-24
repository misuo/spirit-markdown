#ifndef HTML_INLINE_GENERATOR_HPP
#define HTML_INLINE_GENERATOR_HPP

#include <boost/fusion/include/nview.hpp>

#include <boost/spirit/include/karma.hpp>

#include "dom/inlineForwards.hpp"
#include "dom/referenceMap.hpp"

namespace HTML {

namespace karma = boost::spirit::karma;
namespace phx = boost::phoenix;

template <typename Iterator>
struct InlineGenerator : public karma::grammar<Iterator, DOM::Inline()>
{
    InlineGenerator(const DOM::ReferenceMap& refmap_);

    typedef boost::fusion::result_of::as_nview<DOM::ExplicitLink const, 0, 2, 3, 1, 0>::type ExplicitLinkReordered;
    typedef boost::fusion::result_of::as_nview<DOM::ReferenceLink const, 0, 2, 2, 1, 0>::type ReferenceLinkReordered;

    karma::symbols<char, char const*> normalEscapes;
    karma::symbols<char, char const*> quotedEscapes;

    karma::rule<Iterator, DOM::Emph()> emph;
    karma::rule<Iterator, DOM::Strong()> strong;
    karma::rule<Iterator, DOM::InlineHTML()> inlineHTML;
    karma::rule<Iterator, DOM::Code()> code;

    karma::rule<Iterator, bool()> linkOpen, linkClose;

    karma::rule<Iterator, ExplicitLinkReordered()> explicitLinkReordered;
    karma::rule<Iterator, DOM::ExplicitLink()> explicitLink;
    karma::rule<Iterator, ReferenceLinkReordered()> referenceLinkReordered;
    karma::rule<Iterator, DOM::ReferenceLink()> referenceLink, referenceLinkRaw;

    karma::rule<Iterator, DOM::AutoLinkEmail()> autoLinkEmail;
    karma::rule<Iterator, DOM::AutoLinkURL()> autoLinkURL;

    karma::rule<Iterator, std::string()> text, quotedText;

    karma::rule<Iterator, DOM::Inline()> inline_;

    const DOM::ReferenceMap& refmap;

    struct HasRefImpl
    {
        HasRefImpl(const DOM::ReferenceMap& refmap_)
        : refmap(refmap_) {}
        const DOM::ReferenceMap& refmap;

        template <typename Arg>
        struct result
        {
            typedef bool type;
        };

        bool operator()(const DOM::ReferenceLink& rl) const
        {
            using boost::fusion::at_c;

            const std::string& ref(at_c<2>(rl)? *at_c<2>(rl) : at_c<1>(rl));
            return refmap.find(ref) != refmap.end();
        }
    };
    phx::function<HasRefImpl> HasRef;

    struct SourceForImpl
    {
        SourceForImpl(const DOM::ReferenceMap& refmap_)
        : refmap(refmap_) {}
        const DOM::ReferenceMap& refmap;

        template <typename Arg>
        struct result
        {
            typedef std::string type;
        };

        std::string operator()(const ReferenceLinkReordered& rl) const
        {
            using boost::fusion::at_c;

            const std::string& ref(at_c<1>(rl)? *at_c<1>(rl) : at_c<3>(rl));
            DOM::ReferenceMap::const_iterator ri = refmap.find(ref);
            //assert(ri != refmap.end());
            if (ri != refmap.end())
                return at_c<2>(*ri);
            else
                return "##notfound##";
        }
    };

    phx::function<SourceForImpl> SourceFor;

    struct TitleForImpl
    {
        TitleForImpl(const DOM::ReferenceMap& refmap_)
        : refmap(refmap_) {}
        const DOM::ReferenceMap& refmap;

        template <typename Arg>
        struct result
        {
            typedef boost::optional<std::string> type;
        };

        boost::optional<std::string> operator()(const ReferenceLinkReordered& rl) const
        {
            using boost::fusion::at_c;

            const std::string& ref(at_c<1>(rl)? *at_c<1>(rl) : at_c<3>(rl));
            DOM::ReferenceMap::const_iterator ri = refmap.find(ref);
            if(ri != refmap.end())
                return at_c<3>(*ri);
            else
                return std::string("##notfound##");
        }
    };

    phx::function<TitleForImpl> TitleFor;
};

} // namespace HTML {

#endif // INLINE_HTML_GENERATOR_HPP
