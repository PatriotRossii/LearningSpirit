#include <boost/spirit/include/karma.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct matrix_grammar
    : karma::grammar<OutputIterator, std::vector<std::vector<int>>()>
{
    matrix_grammar()
        : matrix_grammar::base_type(matrix)
    {
        using karma::int_;
        using karma::right_align;
        using karma::eol;

        element = right_align(10)[int_];
        row = '|' << *element << '|';
        matrix = row % eol;
    }

    karma::rule<OutputIterator, std::vector<std::vector<int>>()> matrix;
    karma::rule<OutputIterator, std::vector<int>()> row;
    karma::rule<OutputIterator, int()> element;
};

template <typename OutputIterator>
bool generate_matrix(OutputIterator& sink, const std::vector<std::vector<int>>& v)
{
    matrix_grammar<OutputIterator> matrix;
    return karma::generate(
        sink, matrix, v
    );
}

////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\tPrinting integers in a matrix using Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    // here we put the data to generate
    std::vector<std::vector<int> > v;

    // now, generate the size and the contents for the matrix
    std::srand((unsigned int)std::time(NULL));
    std::size_t rows = std::rand() / ((RAND_MAX + 1u) / 10);
    std::size_t columns = std::rand() / ((RAND_MAX + 1u) / 10);

    v.resize(rows);
    for (std::size_t row = 0; row < rows; ++row)
    {
        v[row].resize(columns);
        std::generate(v[row].begin(), v[row].end(), [](){
            return std::rand() / ((RAND_MAX + 1u) / 100);
        });
    }

    // ok, we got the matrix, now print it out
    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);
    if (!generate_matrix(sink, v))
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

    return 0;
}