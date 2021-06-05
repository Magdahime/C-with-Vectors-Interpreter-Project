#include "matrix.hpp"

struct LUMatrices{
    Matrix lower;
    Matrix upper;
};

LUMatrices LUDecomposition(const Matrix& matrix);