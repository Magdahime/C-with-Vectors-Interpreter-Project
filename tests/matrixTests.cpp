#include <gtest/gtest.h>

#include "matrix.hpp"
#include "matrixHelpers.hpp"

TEST(MatrixTest, MatrixTransposeTest) {
  std::vector<double> values = {2, 1, 2, 1};
  std::vector<double> values2 = {2, 2, 1, 1};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  EXPECT_EQ(matrix.transpose(), resultMatrix);
}

TEST(MatrixTest, MatrixDetTest) {
  std::vector<double> values = {1, 2, 2, 1};
  Matrix matrix(2, 2, values);
  EXPECT_EQ(matrix.det(), -3);
}

TEST(MatrixTest, LUDecompositionTEST) {
  std::vector<double> values = {4, 1, 1, 1};
  Matrix matrix(2, 2, values);
  LUMatrices matrices = LUDecomposition(matrix);
  EXPECT_EQ(matrices.lower*matrices.upper, matrix);
}