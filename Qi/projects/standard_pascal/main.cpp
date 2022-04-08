#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

struct program { };

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct pascal_grammar : qi::grammar<Iterator, program(), ascii::space_type>
{
    pascal_grammar(): pascal_grammar::base_type(program)
    {

    }

    qi::rule<Iterator, ::program, ascii::space_type> program;
};