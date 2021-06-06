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
  EXPECT_EQ(std::get<double>(varMap.at(std::make_pair("cena", 0)).value), 20.50);
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
  EXPECT_EQ(std::get<std::string>(varMap.at(std::make_pair("cena", 0)).value), "cena produktu");
  EXPECT_EQ(varMap.at(std::make_pair("cena", 0)).type, Type::String);
}