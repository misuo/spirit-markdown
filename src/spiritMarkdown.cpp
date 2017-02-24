#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/phoenix/bind/bind_function.hpp>
#include <boost/program_options.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/home/phoenix/bind/bind_function.hpp>
#include <boost/spirit/include/karma.hpp>

#include "html/documentGenerator.hpp"
#include "markdown/documentParser.hpp"
#include "parseError.hpp"

namespace qi = boost::spirit::qi;
namespace karma = boost::spirit::karma;
namespace phx = boost::phoenix;
namespace io = boost::iostreams;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

bool processFile( const fs::path& path, bool showParseTree )
{
  using boost::phoenix::arg_names::arg1;

  try
  {
    // map the input file
    io::mapped_file_source src( path );
    if( !src.is_open() )
      throw std::runtime_error( "could not be read" );
    if( !src.size() )
      return true;

    // parsed document & referenceMap
    DOM::Document doc;
    DOM::ReferenceMap refmap;

    // set up input iterators
    typedef const char* InputIterator;
    InputIterator iter = src.begin();
    InputIterator end = src.end();
    Markdown::DocumentParser<InputIterator> par( refmap );

    // set up parser error handling
    InputIterator where = end;
    // qi::on_error<qi::fail>(par.document, phx::ref(where) = qi::_1);

    // parse it
    if( !qi::parse( iter, end, par, doc ) )
      throw ParseError<InputIterator>( src.begin(), end, where, "parser error" );
    if( iter != end )
      throw ParseError<InputIterator>( src.begin(), end, where, "did not parse completely" );

    // set up output
    typedef std::ostream_iterator<char> OutputIterator;
    OutputIterator out( std::cout );
    HTML::DocumentGenerator<OutputIterator> gen( refmap );

    // output the parse tree if required
    if( showParseTree )
    {
      std::cerr << path << " parse tree:\n";
      std::for_each( doc.begin(), doc.end(), std::cerr << arg1 << '\n' );
    }

    // generate it
    if( !karma::generate( out, gen, doc ) )
      throw std::runtime_error( "Failed to generate" );

    return true;
  }
  catch( const std::exception& err )
  {
    std::cerr << path << ": " << err.what() << std::endl;

    return false;
  }
}

int main( int argc, char* argv[] )
{
  using boost::phoenix::arg_names::arg1;

  try
  {
    po::options_description desc( "Allowed options" );
    desc.add_options()( "help", "produce help message" )( "parse-tree", "show parse tree (useful for debugging)" )( "input-file", po::value<std::vector<std::string>>(), "input file" );

    po::positional_options_description p;
    p.add( "input-file", -1 );

    po::variables_map vm;
    po::store( po::command_line_parser( argc, argv ).options( desc ).positional( p ).run(), vm );
    po::notify( vm );

    if( vm.count( "help" ) || !vm.count( "input-file" ) )
    {
      std::cerr << "usage: " << argv[0] << " [options] <markdown file>\n" << desc << std::endl;
      return 1;
    }

    const std::vector<std::string>& inputFiles = vm["input-file"].as<std::vector<std::string>>();

    int errs = std::count_if( inputFiles.begin(), inputFiles.end(), phx::bind( processFile, arg1, vm.count( "parse-tree" ) ) );
    return errs;
  }
  catch( const std::exception& e )
  {
    std::cerr << "error: " << e.what() << "\n";
  }
  catch( ... )
  {
    std::cerr << "Exception of unknown type!\n";
  }

  return 1;
}
