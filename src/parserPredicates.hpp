#ifndef MARKDOWN_TEST_PREDICATES_HPP
#define MARKDOWN_TEST_PREDICATES_HPP

#include <boost/test/predicate_result.hpp>

#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/core/value.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/object/construct.hpp>
//#include <boost/spirit/home/phoenix/core/argument.hpp>
//#include <boost/spirit/home/phoenix/core/value.hpp>
//#include <boost/spirit/home/phoenix/bind/bind_member_function.hpp>
//#include <boost/spirit/home/phoenix/object/construct.hpp>

namespace qi = boost::spirit::qi;

using boost::test_tools::predicate_result;

template <typename Rule, size_t inputLen>
predicate_result Parses(
    Rule rule,
    const char (&input)[inputLen],
    size_t expectedUnparsed = 0
    )
{
    using namespace boost::phoenix::arg_names;
    using boost::phoenix::ref;
    using boost::phoenix::val;
    using boost::phoenix::bind;
    using boost::phoenix::construct;

    predicate_result res(false);
    const char * iter = input;
    const char * end = input + inputLen - 1;

    const char * where = end;
    qi::on_error<qi::fail>(rule, ref(where) = qi::_1);

    if (!qi::parse(iter, end, rule))
    {
        res.message()
            << "Failed to parse, "
            << std::distance(where, end) << " characters still unparsed: "
            << std::string(where, end);
        return res;
    }

    std::advance(iter, expectedUnparsed);
    if (iter != end)
    {
        res.message() << "Did not parse completely. Still unparsed: \""
                      << std::string(iter, end) << "\"";
        return res;
    }
    
    return true;
}

template <typename Rule, size_t inputLen, typename Result>
predicate_result ParsesTo(
    Rule rule,
    const char (&input)[inputLen],
    const Result& expected
    )
{
    using boost::phoenix::ref;
//    using namespace qi::labels;

    const char * iter = input;
    const char * end = input + inputLen - 1;

    const char * where = end;
    qi::on_error<qi::fail>(rule, ref(where) = qi::_1);
    Result seen;

    if (!qi::parse(iter, end, rule, seen))
    {
        predicate_result res(false);
        res.message()
            << "Failed to parse, "
            << std::distance(where, end) << " characters still unparsed: "
            << std::string(where, end);
        return res;
    }
    //std::advance(iter, expectedUnparsed);
    if (iter != end)
    {
        predicate_result res(false);
        res.message() << "Did not parse completely. Still unparsed: \""
                      << std::string(iter, end) << "\"";
        return res;
    }
    if (seen != expected)
    {
        predicate_result res(false);
        res.message() << "Expected: " << expected << ", seen: " << seen;
        return res;
    }
    
    return true;
}

#endif // MARKDOWN_TEST_PREDICATES_HPP
