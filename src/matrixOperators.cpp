#include "matrix.hpp"

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.rows == rhs.rows && lhs.columns == rhs.columns) {
    Matrix resultMatrix(lhs.rows, lhs.columns);
    for (size_t i = 0; i < lhs.values.size(); i++) {
      resultMatrix.values[i] = lhs.values[i] + rhs.values[i];
    }
    return resultMatrix;
  }
  return Matrix();
}

Matrix operator+(const Matrix& lhs, const int64_t number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] + number;
  }
  return returnMatrix;
}
Matrix operator+(const int64_t number, const Matrix& lhs) {
  return operator+(lhs, number);
}

Matrix operator+(const Matrix& lhs, double number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] + number;
  }
  return returnMatrix;
}

Matrix operator+(const double number, const Matrix& lhs) {
  return operator+(lhs, number);
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.rows == rhs.rows && lhs.columns == rhs.columns) {
    Matrix resultMatrix(lhs.rows, lhs.columns);
    for (size_t i = 0; i < lhs.values.size(); i++) {
      resultMatrix.values[i] = lhs.values[i] - rhs.values[i];
    }
    return resultMatrix;
  }
  return Matrix();
}

Matrix operator-(const Matrix& lhs, const int64_t number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] - number;
  }
  return returnMatrix;
}

Matrix operator-(const int64_t number, const Matrix& lhs) {
  return operator-(lhs, number);
}

Matrix operator-(const Matrix& lhs, const double number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] - number;
  }
  return returnMatrix;
}

Matrix operator-(const double number, const Matrix& lhs) {
  return operator-(lhs, number);
}

Matrix operator-(const Matrix& lhs) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = -lhs.values[i];
  }
  return returnMatrix;
}

Matrix operator+(const Matrix& lhs, const int number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] + number;
  }
  return returnMatrix;
}
Matrix operator+(const int number, const Matrix& lhs) {
  return operator+(lhs, number);
}

Matrix operator-(const Matrix& lhs, const int number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); ++i) {
    returnMatrix.values[i] = lhs.values[i] - number;
  }
  return returnMatrix;
}

Matrix operator-(const int number, const Matrix& lhs) {
  return operator-(lhs, number);
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.columns == rhs.rows) {
    Matrix returnMatrix(lhs.rows, rhs.columns);
    for (size_t i = 0; i < lhs.rows; i++) {
      for (size_t j = 0; j < rhs.columns; j++) {
        for (size_t k = 0; k < lhs.columns; k++) {
          returnMatrix.values[i * lhs.columns + j] +=
              lhs.values[i * lhs.columns + k] * rhs.values[k * rhs.columns + j];
        }
      }
    }
    return returnMatrix;
  }
  return Matrix();
}

Matrix operator*(const Matrix& lhs, const int64_t number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] * number;
  }
  return returnMatrix;
}

Matrix operator*(const int64_t number, const Matrix& lhs) {
  return operator*(lhs, number);
}

Matrix operator*(const Matrix& lhs, const int number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] * number;
  }
  return returnMatrix;
}

Matrix operator*(const int number, const Matrix& lhs) {
  return operator*(lhs, number);
}

Matrix operator*(const Matrix& lhs, const double number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] * number;
  }
  return returnMatrix;
}

Matrix operator*(const double number, const Matrix& lhs) {
  return operator*(lhs, number);
}

Matrix operator/(const Matrix& lhs, const Matrix& rhs) {
  if (rhs.isSquare() && lhs.columns == rhs.rows) {
  }
  return Matrix{};
}

Matrix operator/(const Matrix& lhs, const int64_t number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] / number;
  }
  return returnMatrix;
}

Matrix operator/(const Matrix& lhs, const double number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] / number;
  }
  return returnMatrix;
}

Matrix operator/(const Matrix& lhs, const int number) {
  Matrix returnMatrix(lhs.rows, lhs.columns);
  for (size_t i = 0; i < lhs.values.size(); i++) {
    returnMatrix.values[i] = lhs.values[i] / number;
  }
  return returnMatrix;
}