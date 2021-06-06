#include "matrix.hpp"

#include <iostream>
#include <sstream>

#include "matrixHelpers.hpp"

std::string Matrix::toString() const {
  std::stringstream ss;
  ss << "[";
  for (const auto& value : values) {
    ss << std::to_string(value);
    ss << ",";
  }
  ss << "]";
  return ss.str();
}

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

Matrix Matrix::inverse() const {
  double determinant = det();
  if (determinant == 0.0) {
    throw std::runtime_error("This matrix is singular!");
  }
  return (getCofactor() * (1.0 / det())).transpose();
}

Matrix Matrix::getCofactor() const {
  Matrix mat(rows, columns);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < columns; j++) {
      mat.values[i * columns + j] =
          changeSign(i) * changeSign(j) * getSubmatrix(*this, i, j).det();
    }
  }
  return mat;
}

Matrix Matrix::getSubmatrix(Matrix matrix, uint64_t excluding_row,
                            uint64_t excluding_column) const {
  if (excluding_row > rows - 1 || excluding_column > columns - 1)
    throw std::runtime_error(
        "Invalid excluding row or excluding column parameters");
  Matrix mat(matrix.getRows() - 1, matrix.getColumns() - 1);
  size_t counter = 0;
  for (size_t i = 0; i < matrix.getRows(); i++) {
    for (size_t j = 0; j < matrix.getColumns(); j++) {
      if (i == excluding_row || j == excluding_column) continue;
      mat.values[counter] = matrix.values[i * columns + j];
      counter++;
    }
  }
  return mat;
}

int Matrix::changeSign(int64_t number) const {
  if (number % 2 == 0) return 1;
  return -1;
}