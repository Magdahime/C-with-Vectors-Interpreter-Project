#pragma once
#include <vector>
#include <variant>
// TODO IMPLEMENT THIS
class Matrix
{
public:
private:
    unsigned int columns;
    unsigned int rows;
    std::vector<std::variant<double, int>> values;

    friend bool operator==(Matrix const &lhs, Matrix const &rhs)
    {
        return  lhs.values == rhs.values;
    };
};