#include "instantiations.hpp"

#include "markdown/inlineParsers.ipp"

namespace Markdown {

template struct InlineParsers<DefaultInputIterator>;

} // namespace Markdown {
