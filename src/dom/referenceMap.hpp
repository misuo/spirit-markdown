#ifndef DOM_REFERENCE_MAP_HPP
#define DOM_REFERENCE_MAP_HPP

#include <boost/fusion/include/at_c.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/global_fun.hpp>

#include "dom/blocks.hpp"

namespace DOM {

namespace mi = boost::multi_index;

typedef mi::multi_index_container<
    Reference,
    mi::indexed_by<
        mi::hashed_unique<
            mi::global_fun<
                const Reference&, const std::string&, &boost::fusion::at_c<1, Reference>
                >
            >
        >
    > ReferenceMap;

} // namespace DOM {

#endif // DOM_REFERENCE_MAP_HPP
