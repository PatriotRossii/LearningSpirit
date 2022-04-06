#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct hundreds_: qi::symbols<char, unsigned>
{
    hundreds_()
    {
        add
            ("C", 100)
            ("CC", 200)
            ("CCC", 300)
            ("CD", 400)
            ("D", 500)
            ("DC", 600)
            ("DCC", 700)
            ("DCCC", 800)
            ("CM", 900)
        ;
    }
} hundreds;

struct tens_: qi::symbols<char, unsigned>
{
    tens_()
    {
        add
            ("X", 10)
            ("XX", 20)
            ("XXX", 30)
            ("XL", 40)
            ("L", 50)
            ("LX", 60)
            ("LXX", 70)
            ("LXXX", 80)
            ("XC", 90)
        ;
    }
} tens;

struct ones_: qi::symbols<char, unsigned>
{
    ones_()
    {
        add
            ("I", 1)
            ("II", 2)
            ("III", 3)
            ("IV", 4)
            ("V", 5)
            ("VI", 6)
            ("VII", 7)
            ("VIII", 8)
            ("IX", 9)
        ;
    }
} ones;

template <typename Iterator>
struct roman: qi::grammar<Iterator, unsigned()>
{
    roman(): roman::base_type(start)
    {
        using qi::eps;
        using qi::lit;
        using qi::_val;
        using qi::_1;
        using ascii::char_;

        start = eps [_val = 0] >>
            (
                +lit('M') [_val += 1000]
                || hundreds [_val += _1]
                || tens [_val += _1]
                || ones [_val += _1]
            )
        ;
    }

    qi::rule<Iterator, unsigned()> start;
};

int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tRoman Numerals Parser\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Type a Roman Numeral ...or [q or Q] to quit\n\n";

    typedef std::string::const_iterator iterator_type;
    typedef roman<iterator_type> roman;

    roman roman_parser; // Our grammar

    std::string str;
    unsigned result;
    while (std::getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        //[tutorial_roman_grammar_parse
        bool r = parse(iter, end, roman_parser, result);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "result = " << result << std::endl;
            std::cout << "-------------------------\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \": " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
        //]
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
