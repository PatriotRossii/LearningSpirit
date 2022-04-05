#include <boost/spirit/include/qi.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind/bind.hpp>

#include <iostream>

namespace qi = boost::spirit::qi;

namespace client {
    void print(int const& i) {
        std::cout << i << std::endl;
    }

    struct writer {
        void print(int const& i) const {
            std::cout << i << std::endl;
        }
    };

    struct print_action {
        void operator()(int const& i, qi::unused_type, qi::unused_type) const {
            std::cout << i << std::endl;
        }
    };
}

int main() {
    using qi::int_;
    using qi::parse;
    using namespace client;

    {
        const char* first = "{42}", *last = first + std::strlen(first);
        parse(first, last, '{' >> int_[&print] >> '}');
    }

    {
        const char* first = "{43}", *last = first + std::strlen(first);
        parse(first, last, '{' >> int_[print_action()] >> '}');
    }

    {
        using boost::placeholders::_1;

        const char* first = "{44}", *last = first + std::strlen(first);
        parse(first, last, '{' >> int_[boost::bind(&print, _1)] >> '}');
    }

    {
        using boost::placeholders::_1;

        const char* first = "{44}", *last = first + std::strlen(first);
        writer w;
        parse(first, last, '{' >> int_[boost::bind(&writer::print, &w, _1)] >> '}');
    }

    {
        namespace lambda = boost::lambda;
        using lambda::_1;

        const char* first = "{45}", *last = first + std::strlen(first);
        parse(first, last, '{' >> int_[std::cout << _1 << '\n'] >> '}');
    }

    return 0;
}