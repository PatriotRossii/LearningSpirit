#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/adapt_adt.hpp>
#include <boost/spirit/include/support_adapt_adt_attributes.hpp>

#include <iostream>
#include <string>
#include <complex>

// We can leave off the setters as Karma does not need them
BOOST_FUSION_ADAPT_ADT(
    std::complex<double>,
    (bool, bool, obj.imag() != 0, /**/)
    (double, double, obj.real(), /**/)
    (double, double, obj.imag(), /**/)
)

template <typename Iterator>
bool parse_complex(Iterator first, Iterator last, std::complex<double>& c)
{
    using boost::spirit::qi::double_;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;

    double rN = 0.0;
    double iN = 0.0;
    bool r = phrase_parse(first, last,
        (
                '(' >> double_[ref(rN) = _1]
                    >> -(',' >> double_[ref(iN) = _1]) >> ')'
            |   double_[ref(rN) = _1]
        ),
        space);

    if (!r || first != last) // fail if we did not get a full match
        return false;
    c = std::complex<double>(rN, iN);
    return r;
}

template<typename OutputIterator>
bool generate_complex(OutputIterator sink, const std::complex<double>& c)
{
    using boost::spirit::karma::eps;
    using boost::spirit::karma::double_;
    using boost::spirit::karma::_1;
    using boost::spirit::karma::generate;

    #if defined EASIER
    using boost::spirit::karma::omit;
    return generate(sink,
        (
            !double_(0.0) << '(' << double_ << ", " << double_ << ')'
        |   omit[double_] << double_ << omit[double_]
        ),
        c.imag(), c.real(), c.imag()
    );
    #elif defined FUSION
    using boost::spirit::karma::bool_;
    using boost::spirit::karma::true_;
    using boost::spirit::karma::omit;
    return generate(sink,
        (
            &true_ << '(' << double_ << ", " << double_ << ')'
        |   omit[bool_] << double_ << omit[double_]
        ), c
    );
    #else
    return generate(sink,
        (
            eps(c.imag() != 0) <<
            '(' << double_[_1 = c.real()] << ',' << double_[_1 = c.imag()] << ')'
        |   double_[_1 = c.real()]
        )
    );
    #endif
}

int main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tA complex number micro generator for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Give me a complex number of the form r or (r) or (r,i) \n";
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::complex<double> c;
        if (parse_complex(str.begin(), str.end(), c))
        {
            std::cout << "-------------------------\n";

            std::string generated;
            std::back_insert_iterator<std::string> sink(generated);
            if (!generate_complex(sink, c))
            {
                std::cout << "-------------------------\n";
                std::cout << "Generating failed\n";
                std::cout << "-------------------------\n";
            }
            else
            {
                std::cout << "-------------------------\n";
                std::cout << "Generated: " << generated << "\n";
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
