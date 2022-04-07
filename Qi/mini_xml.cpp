#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct mini_xml;

using mini_xml_node = boost::variant<
    boost::recursive_wrapper<mini_xml>, std::string  
>;

struct mini_xml
{
    std::string name;
    std::vector<mini_xml_node> children;
};

BOOST_FUSION_ADAPT_STRUCT(
    mini_xml,
    (std::string, name)
    (std::vector<mini_xml_node>, children)
)

int const tabsize = 4;

void tab(int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << ' ';
}

struct mini_xml_printer
{
    mini_xml_printer(int indent = 0)
      : indent(indent)
    {
    }

    void operator()(mini_xml const& xml) const;

    int indent;
};

struct mini_xml_node_printer : boost::static_visitor<>
{
    mini_xml_node_printer(int indent = 0)
      : indent(indent)
    {
    }

    void operator()(mini_xml const& xml) const
    {
        mini_xml_printer(indent+tabsize)(xml);
    }

    void operator()(std::string const& text) const
    {
        tab(indent+tabsize);
        std::cout << "text: \"" << text << '"' << std::endl;
    }

    int indent;
};

void mini_xml_printer::operator()(mini_xml const& xml) const
{
    tab(indent);
    std::cout << "tag: " << xml.name << std::endl;
    tab(indent);
    std::cout << '{' << std::endl;

    BOOST_FOREACH(mini_xml_node const& node, xml.children)
    {
        boost::apply_visitor(mini_xml_node_printer(indent), node);
    }

    tab(indent);
    std::cout << '}' << std::endl;
}

#ifndef SHORTER
template <typename Iterator>
struct mini_xml_grammar: qi::grammar<Iterator, mini_xml(), ascii::space_type>
{
    mini_xml_grammar(): mini_xml_grammar::base_type(xml)
    {
        using qi::lit;
        using qi::lexeme;
        using ascii::char_;
        using ascii::string;
        using namespace qi::labels;

        using phoenix::at_c;
        using phoenix::push_back;

        text = lexeme[+(char_ - '<') [_val += _1]];
        node = (xml | text) [_val = _1];

        start_tag =
                '<'
            >>  !lit('/')
            >>  lexeme[+(char_ - '>')   [_val += _1]]
            >> '>'
        ;

        end_tag =
                "</"
            >>  lit(_r1)
            >>  '>'
        ;

        xml =
                start_tag [at_c<0>(_val) = _1]
            >>  *node [push_back(at_c<1>(_val), _1)]
            >>  end_tag(at_c<0>(_val))
        ;
    }

    qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
    qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
    qi::rule<Iterator, std::string(), ascii::space_type> text;
    qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
    qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
};
#endif

#ifdef SHORTER
template <typename Iterator>
struct mini_xml_grammar
    : qi::grammar<Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type>
{
    mini_xml_grammar()
        : mini_xml_grammar::base_type(xml, "xml")
    {
        using qi::lit;
        using qi::lexeme;
        using qi::on_error;
        using qi::fail;
        using ascii::char_;
        using ascii::string;
        using namespace qi::labels;

        using phoenix::construct;
        using phoenix::val;

        text %= lexeme[+(char_ - '<')];
        node %= xml | text;

        start_tag %=
                '<'
            >>  !lit('/')
            >   lexeme[+(char_ - '>')]
            >   '>'
        ;

        end_tag =
                "</"
            >   lit(_r1)
            >   '>'
        ;

        xml %=
                start_tag[_a = _1]
            >   *node
            >   end_tag(_a)
        ;

        xml.name("xml");
        node.name("node");
        text.name("text");
        start_tag.name("start_tag");
        end_tag.name("end_tag");

        on_error<fail>
        (
            xml,
            std::cout
                << val("Error! Expecting ")
                << _4
                << val(" here: \"")
                << construct<std::string>(_3, _2)
                << val("\"")
                << std::endl
        );
    }

    qi::rule<Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type> xml;
    qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
    qi::rule<Iterator, std::string(), ascii::space_type> text;
    qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
    qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
};
#endif

int main(int argc, char **argv)
{
    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
        return 1;
    }

    std::string storage; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(storage));

    typedef mini_xml_grammar<std::string::const_iterator> mini_xml_grammar;
    mini_xml_grammar xml; // Our grammar
    mini_xml ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
    bool r = phrase_parse(iter, end, xml, space, ast);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        mini_xml_printer printer;
        printer(ast);
        return 0;
    }
    else
    {
        std::string::const_iterator some = iter + std::min(30, int(end - iter));
        std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \"" << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
}
