#include "matrix.hpp"

#include <iostream>

#include "matrixHelpers.hpp"

Matrix Matrix::transpose() const {
  Matrix returnMatrix(rows, columns);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < columns; j++) {
      returnMatrix.values[i * columns + j] = values[j * columns + i];
    }
  }
  return returnMatrix;
}
double Matrix::det() const {
  LUMatrices matrices = LUDecomposition(*this);
  double determinant = 1.0;
  for (size_t i = 0; i < values.size(); i += (columns + 1)) {
    determinant *= matrices.lower.getValues()[i];
  }
  return determinant;
}
