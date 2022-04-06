#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/stl.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::double_;
using qi::_1;
using ascii::space;
using phoenix::ref;
using phoenix::push_back;

template<typename Iterator>
bool parse_numbers(Iterator first, Iterator last, std::vector<double>& v)
{
    #ifndef SHORT
    #ifndef SHORTEST
    bool r = qi::phrase_parse(first, last,
        (
            double_[push_back(phoenix::ref(v), _1)]
                >> *(',' >> double_[push_back(phoenix::ref(v), _1)])
        ), space
    );
    #endif
    #endif

    #ifdef SHORT
    bool r = qi::phrase_parse(first, last,
        (
            double_[push_back(phoenix::ref(v), _1)] % ','
        ), space
    );
    #endif

    #ifdef SHORTEST
    bool r = qi::phrase_parse(first, last,
        (
            double_ % ','
        ), space, v
    );
    #endif

    if (first != last) return false;
    return r;
}

int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tA comma separated list parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Give me a comma separated list of numbers.\n";
    std::cout << "The numbers will be inserted in a vector of numbers\n";
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::vector<double> v;
        if (parse_numbers(str.begin(), str.end(), v))
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << str << " Parses OK: " << std::endl;

            for (std::vector<double>::size_type i = 0; i < v.size(); ++i)
                std::cout << i << ": " << v[i] << std::endl;

            std::cout << "\n-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}