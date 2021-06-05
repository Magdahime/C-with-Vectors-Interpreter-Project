#include "matrixHelpers.hpp"

#include <cmath>
#include <iostream>

LUMatrices LUDecomposition(const Matrix& matrix) {
  if (!matrix.isSquare()) throw std::runtime_error("Matrix is not square!");
  uint64_t rows = matrix.getRows();
  Matrix lower(rows, rows);
  Matrix upper(rows, rows);
  std::vector<double> lowerValues(pow(rows, 2), 0);
  std::vector<double> upperValues(pow(rows, 2), 0);
  std::vector<double> sourceValues = matrix.getValues();

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < rows; j++) {
      if (j < i)
        lowerValues[j * rows + i] = 0;
      else {
        lowerValues[j * rows + i] = sourceValues[j * rows + i];
        for (size_t k = 0; k < i; k++) {
          lowerValues[j * rows + i] =
              lowerValues[j * rows + i] -
              lowerValues[j * rows + k] * upperValues[k * rows + i];
        }
      }
    }
    for (size_t j = 0; j < rows; j++) {
      if (j < i)
        upperValues[i * rows + j] = 0;
      else if (j == i)
        upperValues[i * rows + j] = 1;
      else {
        upperValues[i * rows + j] =
            sourceValues[i * rows + j] / lowerValues[i * rows + i];
        for (size_t k = 0; k < i; k++) {
          upperValues[i * rows + j] =
              upperValues[i * rows + j] -
              ((lowerValues[i * rows + k] * upperValues[k * rows + j]) /
               lowerValues[i * rows + i]);
        }
      }
    }
  }
  lower.setValues(lowerValues);
  upper.setValues(upperValues);
  return LUMatrices{lower, upper};
}