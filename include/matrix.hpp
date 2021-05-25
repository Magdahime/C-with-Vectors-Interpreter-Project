#pragma once
#include <variant>
#include <vector>
// TODO IMPLEMENT THIS
class Matrix {
 public:
 private:
  unsigned int columns = 0;
  unsigned int rows = 0;
  std::vector<std::variant<double, int>> values;

  friend bool operator==(Matrix const &lhs, Matrix const &rhs) {
    return lhs.values == rhs.values;
  };
};