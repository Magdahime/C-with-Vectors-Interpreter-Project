#pragma once
#include <cstdint>
#include <variant>
#include <vector>
// TODO IMPLEMENT THIS
class Matrix {
 public:
  Matrix() = default;
  Matrix(uint64_t rows, uint64_t columns, std::vector<double>& values)
      : rows(rows), columns(columns), values(values){};
  Matrix(uint64_t rows, uint64_t columns)
      : rows(rows), columns(columns), values(rows * columns, 0.0){};
  bool empty() { return values.empty(); };

 private:
  uint64_t rows = 0;
  uint64_t columns = 0;
  std::vector<double> values;

  friend bool operator==(Matrix const& lhs, Matrix const& rhs) {
    return lhs.values == rhs.values;
  };
  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator+(const Matrix& lhs, const int64_t number);
  friend Matrix operator+(const int64_t number, const Matrix& lhs);
  friend Matrix operator+(const Matrix& lhs, const double number);
  friend Matrix operator+(const double number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator-(const Matrix& lhs, const int64_t number);
  friend Matrix operator-(const int64_t number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs, const double number);
  friend Matrix operator-(const double number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs);
};