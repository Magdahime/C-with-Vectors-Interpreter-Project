#pragma once
#include <variant>
#include <vector>
// TODO IMPLEMENT THIS
class Matrix {
 public:
  Matrix(int rows, int columns, std::vector<std::variant<double, int>> &values)
      : rows(rows), columns(columns), values(values){};
  bool empty() { return values.empty(); };

 private:
  unsigned int rows = 0;
  unsigned int columns = 0;
  std::vector<std::variant<double, int>> values;

  friend bool operator==(Matrix const &lhs, Matrix const &rhs) {
    return lhs.values == rhs.values;
  };
};