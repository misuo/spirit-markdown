#include "blockParser_heading.ipp"
#include "instantiations.hpp"

namespace Markdown {

// This is not really called. Its only purpose is to
// instantiate the constructor of the grammar.
// void instantiateMarkdownParsers()
// {
//     //typedef std::string::const_iterator iterator_type;
//     typedef const char * iterator_type;
//     BlockParsers<iterator_type> g;
// }

template void BlockParser<DefaultInputIterator>::constructHeadingParsers();

} // namespace Markdown {
