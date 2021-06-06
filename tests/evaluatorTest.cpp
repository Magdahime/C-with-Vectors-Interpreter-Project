#include <gtest/gtest.h>

#include "helpers/exception.hpp"
#include "interpreter/evaluator.hpp"
#include "parser/parser.hpp"

TEST(EvaluatorTest, EvaluateVariableTest) {
  try {
    Evaluator evaluator;
    IdentifierNode node(Token(Token::TokenType::IdentifierToken, "zmienna"));
    evaluator.evaluate(&node);
    FAIL() << "Expected throw!";
  } catch (const SemanticError& err) {
    EXPECT_EQ(err.what(), std::string("Unknown variable: zmienna at:1:1"));
  } catch (...) {
    FAIL() << "Invalid throw!";
  }
}

TEST(EvaluatorTest, EvaluateVariableTest2) {
  Evaluator evaluator;
  evaluator.enterVariable("zmienna", TokenVariant(4));
  EXPECT_EQ(evaluator.getVariableMap().size(), 1);
  IdentifierNode node(Token(Token::TokenType::IdentifierToken, "zmienna"));
  EXPECT_EQ(std::get<int64_t>(evaluator.evaluate(&node)), 4);
}

TEST(EvaluatorTest, EvaluateVariableTest3) {
  Evaluator evaluator;
  ValueNode node(Token(Token::TokenType::IntegerLiteralToken, 1));
  EXPECT_EQ(std::get<int64_t>(evaluator.evaluate(&node)), 1);
}

TEST(EvaluatorTest, EvaluateVariableTest4) {
  Evaluator evaluator;
  ValueNode node(Token(Token::TokenType::StringLiteralToken, "zmienna"));
  EXPECT_EQ(std::get<std::string>(evaluator.evaluate(&node)), "zmienna");
}

TEST(EvaluatorTest, EvaluateVariableTest5) {
  Evaluator evaluator;
  ValueNode node(Token(Token::TokenType::DoubleLiteralToken, 3.14));
  EXPECT_EQ(std::get<double>(evaluator.evaluate(&node)), 3.14);
}

TEST(EvaluatorTest, INT_INT_TEST) {
  std::string test = R"(2+2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 4);
}

TEST(EvaluatorTest, DOUBLE_DOUBLE_TEST) {
  std::string test = R"(3.0+0.14)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 3.14);
}

TEST(EvaluatorTest, INT_DOUBLE_TEST) {
  std::string test = R"(3 +0.14)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 3.14);
}

TEST(EvaluatorTest, DOUBLE_INT_TEST) {
  std::string test = R"(3.14 + 1)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 3.14 + 1);
}

TEST(EvaluatorTest, STRING_STRING_TEST) {
  std::string test = R"("mama" + "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<std::string>(node->accept(evaluator)),
            std::string("mamatata"));
}

TEST(EvaluatorTest, STRING_STRING_TEST2) {
  try {
    std::string test = R"("mama" - "tata")";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    Evaluator evaluator;
    ExpressionNodeUptr node = parser.parseExpression();
    node->accept(evaluator);
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, MATRIX_INT_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna + 1)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, INT_MATRIX_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(1+zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, DOUBLE_MATRIX_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2.5, 2.5, 2.5, 2.5};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(1.5+zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, MATRIX_DOUBLE_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2.5, 2.5, 2.5, 2.5};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna+1.5)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, MATRIX_MATRIX_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  Matrix matrix2(2, 2, values);

  evaluator.enterVariable("matrix1", matrix);
  evaluator.enterVariable("matrix2", matrix2);
  std::string test = R"(matrix1 + matrix2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, INT_INT_TEST2) {
  std::string test = R"(2*2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 4);
}

TEST(EvaluatorTest, INT_INT_TEST3) {
  std::string test = R"(2/2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, DOUBLE_DOUBLE_TEST2) {
  std::string test = R"(3.0*3.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 9.0);
}

TEST(EvaluatorTest, DOUBLE_DOUBLE_TEST3) {
  std::string test = R"(3.0/3.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 1.0);
}

TEST(EvaluatorTest, INT_DOUBLE_TEST2) {
  std::string test = R"(3 * 4.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 12.0);
}

TEST(EvaluatorTest, INT_DOUBLE_TEST3) {
  std::string test = R"(3/4.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 0.75);
}

TEST(EvaluatorTest, DOUBLE_INT_TEST2) {
  std::string test = R"(3.0 * 4)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 12.0);
}

TEST(EvaluatorTest, DOUBLE_INT_TEST3) {
  std::string test = R"(3.0 / 4 )";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 0.75);
}

TEST(EvaluatorTest, MATRIX_INT_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna * 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, MATRIX_INT_TEST3) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values);
  Matrix matrix(2, 2, values2);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna/2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, INT_MATRIX_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(2*zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, INT_MATRIX_TEST3) {
  try {
    Evaluator evaluator;
    std::vector<double> values = {1, 1, 1, 1};
    std::vector<double> values2 = {2.5, 2.5, 2.5, 2.5};
    Matrix resultMatrix(2, 2, values2);
    Matrix matrix(2, 2, values);
    evaluator.enterVariable("zmienna", matrix);
    std::string test = R"(2/zmienna)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    ExpressionNodeUptr node = parser.parseExpression();
    node->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, DOUBLE_MATRIX_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2.0, 2.0, 2.0, 2.0};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(2.0 * zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, DOUBLE_MATRIX_TEST3) {
  try {
    Evaluator evaluator;
    std::vector<double> values = {1, 1, 1, 1};
    std::vector<double> values2 = {2.5, 2.5, 2.5, 2.5};
    Matrix resultMatrix(2, 2, values2);
    Matrix matrix(2, 2, values);
    evaluator.enterVariable("zmienna", matrix);
    std::string test = R"(2.0/zmienna)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    ExpressionNodeUptr node = parser.parseExpression();
    node->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, MATRIX_DOUBLE_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2.5, 2.5, 2.5, 2.5};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna*2.5)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), resultMatrix);
}

TEST(EvaluatorTest, MATRIX_DOUBLE_TEST3) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(zmienna/2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), matrix / 2.0);
}

TEST(EvaluatorTest, MATRIX_MATRIX_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2.5, 3, 4, 2};

  Matrix matrix(2, 2, values);
  Matrix matrix2(2, 2, values2);

  evaluator.enterVariable("matrix1", matrix);
  evaluator.enterVariable("matrix2", matrix2);
  std::string test = R"(matrix1 * matrix2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), matrix * matrix2);
}

TEST(EvaluatorTest, EXP_INT_INT_TEST) {
  std::string test = R"(2^2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 4);
}

TEST(EvaluatorTest, EXP_DOUBLE_DOUBLE_TEST) {
  std::string test = R"(3.0^3.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 27.0);
}

TEST(EvaluatorTest, EXP_INT_DOUBLE_TEST) {
  std::string test = R"(3 ^ 4.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 81.0);
}

TEST(EvaluatorTest, EXP_DOUBLE_INT_TEST) {
  std::string test = R"(3.0 ^ 4)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 81.0);
}

TEST(EvaluatorTest, Evaluate_MatrixOperator_TEST) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(det zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 0.0);
}

TEST(EvaluatorTest, Evaluate_MatrixOperator_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(det zmienna + zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<double>(node->accept(evaluator)), 0.0);
}

TEST(EvaluatorTest, Evaluate_MatrixOperator_TEST3) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 2, 1};
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(inv zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), matrix.inverse());
}

TEST(EvaluatorTest, Evaluate_MatrixOperator_TEST4) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  Matrix matrix(2, 2, values);
  evaluator.enterVariable("zmienna", matrix);
  std::string test = R"(trans zmienna)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseExpression();
  EXPECT_EQ(std::get<Matrix>(node->accept(evaluator)), matrix.transpose());
}

TEST(EvaluatorTest, Evaluate_MatrixOperator_TEST5) {
  try {
    Evaluator evaluator;
    evaluator.enterVariable("zmienna", 2.0);
    std::string test = R"(det zmienna)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    ExpressionNodeUptr node = parser.parseExpression();
    node->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST) {
  std::string test = R"(2 > 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST2) {
  std::string test = R"(2>=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST3) {
  std::string test = R"(2<2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST4) {
  std::string test = R"(2<=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST5) {
  std::string test = R"(2!=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST6) {
  std::string test = R"(2==2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST7) {
  std::string test = R"(2 and 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_INT_TEST8) {
  std::string test = R"(2 or 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST) {
  std::string test = R"(2.0 > 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST2) {
  std::string test = R"(2.0>=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST3) {
  std::string test = R"(2.0<2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST4) {
  std::string test = R"(2.0<=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST5) {
  std::string test = R"(2.0!=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST6) {
  std::string test = R"(2.0==2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST7) {
  std::string test = R"(2.0 and 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_DOUBLE_TEST8) {
  std::string test = R"(2.0 or 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST) {
  std::string test = R"(2.0 > 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST2) {
  std::string test = R"(2.0>=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST3) {
  std::string test = R"(2.0<2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST4) {
  std::string test = R"(2.0<=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST5) {
  std::string test = R"(2.0!=2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST6) {
  std::string test = R"(2.0==2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST7) {
  std::string test = R"(2.0 and 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_DOUBLE_INT_TEST8) {
  std::string test = R"(2.0 or 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST) {
  std::string test = R"(2 > 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST2) {
  std::string test = R"(2>=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST3) {
  std::string test = R"(2<2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST4) {
  std::string test = R"(2<=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST5) {
  std::string test = R"(2!=2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}
TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST6) {
  std::string test = R"(2==2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST7) {
  std::string test = R"(2 and 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_INT_DOUBLE_TEST8) {
  std::string test = R"(2 or 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST) {
  std::string test = R"("mama" > "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST2) {
  std::string test = R"("mama" >= "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST3) {
  std::string test = R"("mama" < "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST4) {
  std::string test = R"("mama" <= "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST5) {
  std::string test = R"("mama" != "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_STRING_STRING_TEST6) {
  std::string test = R"("mama" == "tata")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_MATRIX_MATRIX_TEST7) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  Matrix matrix2(2, 2, values2);

  evaluator.enterVariable("matrix1", matrix);
  evaluator.enterVariable("matrix2", matrix2);
  std::string test = R"(matrix1 == matrix2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_MATRIX_MATRIX_TEST2) {
  Evaluator evaluator;
  std::vector<double> values = {1, 1, 1, 1};
  std::vector<double> values2 = {2, 2, 2, 2};
  Matrix resultMatrix(2, 2, values2);
  Matrix matrix(2, 2, values);
  Matrix matrix2(2, 2, values2);

  evaluator.enterVariable("matrix1", matrix);
  evaluator.enterVariable("matrix2", matrix2);
  std::string test = R"(matrix1 != matrix2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, LOGICALOP_MATRIX_MATRIX_TEST3) {
  try {
    Evaluator evaluator;
    std::vector<double> values = {1, 1, 1, 1};
    std::vector<double> values2 = {2, 2, 2, 2};
    Matrix resultMatrix(2, 2, values2);
    Matrix matrix(2, 2, values);
    Matrix matrix2(2, 2, values);

    evaluator.enterVariable("matrix1", matrix);
    evaluator.enterVariable("matrix2", matrix2);
    std::string test = R"(matrix1 > matrix2)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
    node->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, LOGICALOP_NOT_INT_TEST) {
  std::string test = R"(not 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_NOT_INT_TEST2) {
  std::string test = R"(! 2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_NOT_DOUBLE_TEST) {
  std::string test = R"(not 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, LOGICALOP_NOT_DOUBLE_TEST2) {
  std::string test = R"(! 2.0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 0);
}

TEST(EvaluatorTest, MIX_LOGICAL_NOT_LOGICAL_OP) {
  std::string test = R"(3+ 2 > 0)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  Evaluator evaluator;
  ExpressionNodeUptr node = parser.parseMultipleTestExpressions();
  EXPECT_EQ(std::get<int64_t>(node->accept(evaluator)), 1);
}

TEST(EvaluatorTest, EvaluateAssignment) {
  std::string test = R"(integer wiek = 2021 - 1999)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("wiek", 0)).value), 22);
  EXPECT_EQ(varMap.at(std::make_pair("wiek", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, EvaluateAssignment2) {
  try {
    std::string test = R"(integer cena = 20.50)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    parser.parseProgram();
    Evaluator evaluator;
    parser.getProgramNode()->accept(evaluator);
    FAIL() << "Expect SemanticError throw";
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL() << "Expect SemanticError throw";
  }
}

TEST(EvaluatorTest, EvaluateAssignment3) {
  std::string test = R"(double cena = 20.50)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<double>(varMap.at(std::make_pair("cena", 0)).value),
            20.50);
  EXPECT_EQ(varMap.at(std::make_pair("cena", 0)).type, Type::Double);
}

TEST(EvaluatorTest, EvaluateAssignment4) {
  std::string test = R"(text cena = "cena produktu")";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<std::string>(varMap.at(std::make_pair("cena", 0)).value),
            "cena produktu");
  EXPECT_EQ(varMap.at(std::make_pair("cena", 0)).type, Type::String);
}

TEST(EvaluatorTest, EvaluateMatrixAssignment) {
  std::string test = R"(matrix[2][2] macierz = [0])";
  Matrix matrix(2, 2);
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<Matrix>(varMap.at(std::make_pair("macierz", 0)).value),
            matrix);
  EXPECT_EQ(varMap.at(std::make_pair("macierz", 0)).type, Type::Matrix);
}

TEST(EvaluatorTest, EvaluateMatrixAssignment2) {
  try {
    std::string test = R"(matrix[2][2] macierz = [1,2,3,4,5])";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    parser.parseProgram();
    Evaluator evaluator;
    parser.getProgramNode()->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(EvaluatorTest, EvaluateMatrixAssignment3) {
  std::string test = R"(matrix[2][2] macierz = [1,2,3,4])";
  std::vector<double> vec = {1, 2, 3, 4};
  Matrix matrix(2, 2, vec);
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<Matrix>(varMap.at(std::make_pair("macierz", 0)).value),
            matrix);
  EXPECT_EQ(varMap.at(std::make_pair("macierz", 0)).type, Type::Matrix);
}

TEST(EvaluatorTest, EvaluateMatrixAssignment4) {
  std::string test = R"(integer zmienna = 4
matrix[2][2] macierz = [zmienna,zmienna,zmienna,zmienna])";
  std::vector<double> vec = {4, 4, 4, 4};
  Matrix matrix(2, 2, vec);
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<Matrix>(varMap.at(std::make_pair("macierz", 0)).value),
            matrix);
  EXPECT_EQ(varMap.at(std::make_pair("macierz", 0)).type, Type::Matrix);
}

TEST(EvaluatorTest, EvaluateIFTest) {
  std::string test = R"(integer zmienna = 4
if(zmienna > 0):
  zmienna = 8
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            8);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, EvaluateIFOtherwiseTest) {
  std::string test = R"(integer zmienna = 4
if(zmienna < 0):
  zmienna = 2
otherwise:
  zmienna = 9
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            9);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, LocalityOfVariables) {
  std::string test = R"(integer zmienna = 4
if(zmienna < 0):
  integer zmienna = 2
otherwise:
  integer zmienna = 9
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            4);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, LocalityOfVariables2) {
  std::string test = R"(integer zmienna = 4
if(zmienna < 0):
  integer zmienna = 2
  zmienna = 3
otherwise:
  integer zmienna = 9
  zmienna = 7
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            4);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, EvaluateLoopTest) {
  std::string test = R"(integer zmienna = 0
loop(0:10:2):
  zmienna = zmienna + 1
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            5);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, EvaluateLoopTest2) {
  std::string test = R"(integer licznik = 5
integer zmienna = 0
loop(0:2*licznik:2):
  zmienna = zmienna + 1
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            5);
  EXPECT_EQ(varMap.at(std::make_pair("zmienna", 0)).type, Type::Integer);
}

TEST(EvaluatorTest, EvaluateLoopTest3) {
  try {
    std::string test = R"(integer licznik = 5
integer zmienna = 0
loop(0:2*licznik:-2):
  zmienna = zmienna + 1
)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    parser.parseProgram();
    Evaluator evaluator;
    parser.getProgramNode()->accept(evaluator);
    FAIL() << "Expected throw";
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL() << "Expected semantic error throw";
  }
}

TEST(EvaluatorTest, EvaluateAsLongAsTest) {
  std::string test = R"(integer licznik = 5
integer zmienna = 1
asLongAs(licznik > 0):
  zmienna = zmienna * 2
  licznik = licznik - 1
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            32);
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("licznik", 0)).value),
            0);
}

TEST(EvaluatorTest, EvaluateAsLongAsTest2) {
  std::string test = R"(integer licznik = 5
integer zmienna = 1
asLongAs(licznik > 0 and zmienna < 10):
  zmienna = zmienna * 2
  licznik = licznik - 1
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("zmienna", 0)).value),
            16);
  EXPECT_EQ(std::get<int64_t>(varMap.at(std::make_pair("licznik", 0)).value),
            1);
}

TEST(EvaluatorTest, ConditionCaseTest) {
  std::string test = R"(integer age = 60
double price = 0.0
condition:
    case(age < 10):
        price = 5.0
    case(age >= 10 and age < 20):
        price = 15.0
    case(age >= 20 and age < 60):
        price = 25.0
    case(age >= 60):
        price = 15.0
    default: // tutaj wpadamy gdy nic nie pasuje
        price = -1.0
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<double>(varMap.at(std::make_pair("price", 0)).value),
            15.0);
}

TEST(EvaluatorTest, ConditionCaseTest3) {
  std::string test = R"(integer age = 60
double price = 0.0
condition:
    case(age < 10):
        price = 5.0
    case(age >= 10 and age < 20):
        price = 15.0
    case(age >= 20):
        price = 25.0
    case(age == 60):
        price = 35.0
    default: // tutaj wpadamy gdy nic nie pasuje
        price = -1.0
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto varMap = evaluator.getVariableMap();
  EXPECT_EQ(std::get<double>(varMap.at(std::make_pair("price", 0)).value),
            25.0);
}

TEST(EvaluatorTest, EvaluateFunctionStatementTest) {
  std::string test = R"(
void function concat(text tekst1, text tekst2):
  return tekst1 + tekst2)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  Evaluator evaluator;
  parser.getProgramNode()->accept(evaluator);
  auto funMap = evaluator.getFunctionMap();
  auto funInfo = funMap.at("concat").second;
  EXPECT_EQ(funInfo.returnType, Type::Void);
  EXPECT_EQ(funInfo.arguments.size(), 2);
  EXPECT_EQ(funInfo.arguments[0].type, Type::String);
  EXPECT_EQ(funInfo.arguments[0].identifier, "tekst1");
  EXPECT_EQ(funInfo.arguments[1].type, Type::String);
  EXPECT_EQ(funInfo.arguments[1].identifier, "tekst2");
}

TEST(EvaluatorTest, EvaluateFunctionStatementTest2) {
  try {
    std::string test = R"(
void function concat(text tekst1, text tekst2):
  return tekst1 + tekst2
integer function concat(integer int1, integer int2):
  return int1 + int2)";
    StringSource src(test);
    LexicalAnalyzer lexicAna(&src);
    Parser parser(lexicAna);
    parser.parseProgram();
    Evaluator evaluator;
    parser.getProgramNode()->accept(evaluator);
    FAIL();
  } catch (const SemanticError& err) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}
