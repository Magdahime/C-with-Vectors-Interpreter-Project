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
  EXPECT_EQ(matrices.lower * matrices.upper, matrix);
}

TEST(MatrixTest, CreateSubmatrixTest) {
  std::vector<double> values = {4, 1, 1, 1};
  std::vector<double> returnValues = {1};
  Matrix returnMatrix(1, 1, returnValues);
  Matrix matrix(2, 2, values);
  EXPECT_EQ(matrix.getSubmatrix(matrix, 0, 0), returnMatrix);
}

TEST(MatrixTest, CreateSubmatrixTest2) {
  std::vector<double> values = {1, 1, 1, 2, 2, 2, 3, 3, 3};
  std::vector<double> returnValues = {2, 2, 3, 3};
  Matrix returnMatrix(2, 2, returnValues);
  Matrix matrix(3, 3, values);
  EXPECT_EQ(matrix.getSubmatrix(matrix, 0, 0), returnMatrix);
}

TEST(MatrixTest, CreateSubmatrixTest3) {
  std::vector<double> values = {1, 1, 1, 2, 2, 2, 3, 3, 3};
  std::vector<double> returnValues = {1, 1, 3, 3};
  Matrix returnMatrix(2, 2, returnValues);
  Matrix matrix(3, 3, values);
  EXPECT_EQ(matrix.getSubmatrix(matrix, 1, 1), returnMatrix);
}

// TEST(MatrixTest, CofactorTest) {
//   std::vector<double> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//   std::vector<double> cofValues = {-3, 6, -3, 6, -12, 6, -3, 6, -3};
//   Matrix returnMatrix(3, 3, cofValues);
//   Matrix matrix(3, 3, values);
//   auto cofactored = matrix.getCofactor();
//   EXPECT_EQ(cofactored, returnMatrix);
// }