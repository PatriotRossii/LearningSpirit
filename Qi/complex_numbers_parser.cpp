#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <iostream>
#include <string>
#include <complex>

namespace client {
    template <typename Iterator>
    bool parse_complex(Iterator first, Iterator last, std::complex<double>& c)
    {
        using boost::spirit::qi::double_;
        using boost::spirit::qi::_1;
        using boost::spirit::qi::phrase_parse;
        using boost::spirit::ascii::space;
        using boost::phoenix::ref;

        double rN = 0.0, iN = 0.0;
        bool r = phrase_parse(first, last,
            (
                '(' >> double_[ref(rN) = _1]
                    >> -(',' >> double_[ref(iN) = _1]) >> ')'
                | double_[ref(rN) = _1]
            ), space
        );

        if(!r || first != last)
            return false;
        c = std::complex<double>(rN, iN);
        return r;
    }
}

int main() {
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tA complex number micro parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Give me a complex number of the form r or (r) or (r,i) \n";
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    while(std::getline(std::cin, str)) {
        if(str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::complex<double> c;
        if(client::parse_complex(str.begin(), str.end(), c))
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "got: " << c << std::endl;
            std::cout << "\n-------------------------\n";
        } else {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}