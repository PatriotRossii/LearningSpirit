#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

#include <iostream>
#include <string>
#include <vector>

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last, std::vector<double>& v)
{
    using boost::spirit::qi::double_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;

    bool r = phrase_parse(first, last, double_ % ',', space, v);
    if (first != last)
        return false;
    return r;
}

struct complex
{
    complex(double a, double b = 0.0): a(a), b(b) {}
    double a, b;
};

std::ostream& operator<<(std::ostream& os, const complex& z)
{
    os << "{" << z.a << "," << z.b << "}";
    return os;
}

template <typename OutputIterator, typename Container>
bool generate_numbers(OutputIterator& sink, const Container& v)
{
    using boost::spirit::karma::stream;
    using boost::spirit::karma::generate;
    using boost::spirit::karma::eol;

    bool r = generate(sink, stream % ", ", v);
    return r;
}

int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\tA comma separated list generator for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Give me a comma separated list of numbers.\n";
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::vector<double> v;      // here we put the data gotten from input
        if (parse_numbers(str.begin(), str.end(), v))
        {
            // ok, we got some numbers, fill a vector of complex 
            // instances and print them back out
            std::vector<complex> vc;
            std::vector<double>::const_iterator end = v.end();
            for (std::vector<double>::const_iterator it = v.begin(); 
                 it != end; ++it)
            {
                double real(*it);
                if (++it != end)
                    vc.push_back(complex(real, *it));
                else {
                    vc.push_back(complex(real));
                    break;
                }
            }

            std::cout << "-------------------------\n";

            std::string generated;
            std::back_insert_iterator<std::string> sink(generated);
            if (!generate_numbers(sink, vc))
            {
                std::cout << "-------------------------\n";
                std::cout << "Generating failed\n";
                std::cout << "-------------------------\n";
            }
            else
            {
                std::cout << "-------------------------\n";
                std::cout << "Generated:\n" << generated << "\n";
                std::cout << "-------------------------\n";
            }
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
