#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/operator.hpp>

#include <string>

struct program { };

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct pascal_grammar : qi::grammar<Iterator, program(), ascii::space_type>
{
    pascal_grammar(): pascal_grammar::base_type(program)
    {
        /*
            6.1.1 General
        */

        letter %=
                qi::lit('a') | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j'
            |   'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't'
            |   'u' | 'v' | 'w' | 'x' | 'y' | 'z'
        ;
        digit %=
                qi::lit('0') | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        ;

        /*
            6.1.2 Special-symbols
        */

        special_symbol %=
                qi::lit('+') | '-' | '*' | '/' | '=' | '<' | '>' | '[' | ']'
            |   '.' | ',' | ':' | ';' | '@' | '(' | ')'
            |   "<>" | "<=" | ">=" | ":=" | ".." | word_symbol
        ;
        word_symbol %=
                qi::lit("and") | "array" | "begin" | "case" | "const" | "div"
            |   "do" | "downto" | "else" | "end" | "file" | "for"
            |   "function" | "goto" | "if" | "in" | "label" | "mod"
            |   "nil" | "not" | "of" | "or" | "packed" | "procedure"
            |   "program" | "record" | "repeat" | "set" | "then"
            |   "to" | "type" | "until" | "var" | "while" | "with"
        ;

        /*
            6.1.3 Identifiers
        */

        identifier %=
            letter < *(letter | digit)
        ;

        /*
            6.1.4 Directives
        */

        directive %=
            letter < *(letter | digit)
        ;

        /*
            6.1.5 Numbers
        */

        signed_number %=
            signed_integer | signed_real
        ;
        signed_real %=
            (-sign) > unsigned_real
        ;
        signed_integer %=
            (-sign) > unsigned_integer
        ;
        unsigned_number %=
            unsigned_integer | unsigned_real
        ;
        sign %=
            qi::lit('+') | '-'
        ;
        unsigned_real %=
                digit_sequence > '.' > fractional_part
            >   -('e' > scale_factor)
        ;
        unsigned_integer %=
            digit_sequence
        ;
        fractional_part %=
            digit_sequence
        ;
        scale_factor %=
            (-sign) > digit_sequence
        ;
        digit_sequence %=
            digit > *digit
        ;

        /*
            6.1.6 Labels
        */

        label %=
            digit_sequence
        ;

        /*
            6.1.7 Character-strings
        */

        character_string %=
            '\'' > string_element > *(string_element) > '\''
        ;
        string_element %=
            apostrophe_image | string_character
        ;
        apostrophe_image %=
            qi::lit("''")
        ;
        string_character %= qi::print();
    }

    qi::rule<Iterator, ::program, ascii::space_type> program;

    qi::rule<Iterator, char()> letter;
    qi::rule<Iterator, char()> digit;

    qi::rule<Iterator, std::string()> special_symbol;
    qi::rule<Iterator, std::string()> word_symbol;
    qi::rule<Iterator, std::string()> identifier;
    qi::rule<Iterator, std::string()> directive;

    qi::rule<Iterator, std::string()> signed_number;
    qi::rule<Iterator, std::string()> signed_real;
    qi::rule<Iterator, std::string()> signed_integer;
    qi::rule<Iterator, std::string()> unsigned_number;
    qi::rule<Iterator, char()> sign;
    qi::rule<Iterator, std::string()> unsigned_real;
    qi::rule<Iterator, std::string()> unsigned_integer;
    qi::rule<Iterator, std::string()> fractional_part;
    qi::rule<Iterator, std::string()> scale_factor;
    qi::rule<Iterator, std::string()> digit_sequence;

    qi::rule<Iterator, std::string()> label;

    qi::rule<Iterator, std::string()> character_string;
    qi::rule<Iterator, std::string()> string_element;
    qi::rule<Iterator, std::string()> apostrophe_image;
    qi::rule<Iterator, char()> string_character;
};