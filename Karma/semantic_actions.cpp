#include <boost/spirit/include/karma.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind/bind.hpp>

#include <iostream>
#include <sstream>

void read_function(int& i)
{
    i = 42;
}

int main() {
    using boost::spirit::karma::int_;
    using boost::spirit::karma::generate;

    {
        using namespace boost::spirit;

        std::string generated;
        std::back_insert_iterator<std::string> outiter(generated);

        generate(outiter, '{' << int_[&read_function] << '}');
        std::cout << "Result: " << generated << std::endl;
    }
}