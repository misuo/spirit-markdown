#include "instantiations.hpp"

#include "markdown/stringParsers.ipp"

namespace Markdown {

// This is not really called. Its only purpose is to
// instantiate the constructor of the grammar.
// void instantiateStringsGrammar()
// {
//     typedef std::string::const_iterator iterator_type;
//     StringsGrammar<iterator_type> g;
// }

template struct StringParsers<DefaultInputIterator>;

} // namespace Markdown {
