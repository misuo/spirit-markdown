#pragma once

#include <iterator>

// All grammars will be instantiated with this iterator:
typedef const char * DefaultInputIterator;

// All generators will be instantiated with this iterator type:
typedef std::ostream_iterator<char> DefaultOutputIterator;