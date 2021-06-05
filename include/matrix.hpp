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
  bool empty() const { return values.empty(); };
  bool isSquare() const { return columns == rows; }
  const std::vector<double>& getValues() const { return values; }
  void setValues(std::vector<double>& values) { this->values = values; }
  uint64_t getColumns() const { return columns; }
  uint64_t getRows() const { return rows; }

  Matrix transpose() const;
  double det() const;
  Matrix inverse() const;

 private:
  uint64_t rows = 0;
  uint64_t columns = 0;
  std::vector<double> values;

  friend bool operator==(Matrix const& lhs, Matrix const& rhs) {
    return lhs.values == rhs.values && lhs.rows == rhs.rows &&
           lhs.columns == rhs.columns;
  };

  // Additive
  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator+(const Matrix& lhs, const int64_t number);
  friend Matrix operator+(const int64_t number, const Matrix& lhs);
  friend Matrix operator+(const Matrix& lhs, const int number);
  friend Matrix operator+(const int number, const Matrix& lhs);
  friend Matrix operator+(const Matrix& lhs, const double number);
  friend Matrix operator+(const double number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator-(const Matrix& lhs, const int64_t number);
  friend Matrix operator-(const int64_t number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs, const int number);
  friend Matrix operator-(const int number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs, const double number);
  friend Matrix operator-(const double number, const Matrix& lhs);
  friend Matrix operator-(const Matrix& lhs);
  // Multiplicative
  friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator*(const Matrix& lhs, const int64_t number);
  friend Matrix operator*(const int64_t number, const Matrix& lhs);
  friend Matrix operator*(const Matrix& lhs, const int number);
  friend Matrix operator*(const int number, const Matrix& lhs);
  friend Matrix operator*(const Matrix& lhs, const double number);
  friend Matrix operator*(const double number, const Matrix& lhs);
  friend Matrix operator/(const Matrix& lhs, const Matrix& rhs);
  friend Matrix operator/(const Matrix& lhs, const int64_t number);
  friend Matrix operator/(const Matrix& lhs, const double number);
  friend Matrix operator/(const Matrix& lhs, const int number);
};