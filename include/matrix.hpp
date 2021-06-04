#pragma once
#include <variant>
#include <vector>
// TODO IMPLEMENT THIS
class Matrix {
 public:
 Matrix() = default;
  Matrix(int rows, int columns, std::vector<double>& values)
      : rows(rows), columns(columns), values(values){};
  Matrix(int rows, int columns)
      : rows(rows), columns(columns), values(rows * columns, 0.0){};
  bool empty() { return values.empty(); };

 private:
  unsigned int rows = 0;
  unsigned int columns = 0;
  std::vector<double> values;

  friend bool operator==(Matrix const& lhs, Matrix const& rhs) {
    return lhs.values == rhs.values;
  };
  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator+(const Matrix& lhs, const int number);
  friend Matrix operator+(const int number, const Matrix& lhs);
  friend Matrix operator+(const Matrix& lhs, double number);
  friend Matrix operator+(double number, const Matrix& lhs);
};