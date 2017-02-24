#include "instantiations.hpp"

#include "markdown/htmlParsers.ipp"

namespace Markdown {

// This is not really called. Its only purpose is to
// instantiate the constructor of the grammar.
// void instantiateMarkdownStrings()
// {
//     typedef std::string::const_iterator iterator_type;
//     MarkdownStrings<iterator_type> g;
// }

template struct HTMLParsers<DefaultInputIterator>;

} // namespace Markdown {
