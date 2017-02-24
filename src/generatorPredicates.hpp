#pragma once

#include <boost/spirit/include/karma.hpp>
#include <boost/test/predicate_result.hpp>

#include <sstream>

#include "instantiations.hpp"

namespace karma = boost::spirit::karma;

using boost::test_tools::predicate_result;

template <typename Rule, typename Attr>
predicate_result Generates(const Rule &rule, const Attr &attr, const std::string &expected)
{
  predicate_result res(false);

  std::ostringstream ss;
  DefaultOutputIterator out(ss);

  if( !karma::generate(out, rule, attr) )
  {
    res.message() << "Failed to generate";
    return res;
  }

  if( ss.str() != expected )
  {
    res.message() << "Expected: \'" << expected << "\', seen: \'" << ss.str() << "\'";
    return res;
  }

  return true;
}
