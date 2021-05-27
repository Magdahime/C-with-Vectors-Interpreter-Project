#include <gtest/gtest.h>

#include "parser/node.hpp"
#include "parser/parser.hpp"

TEST(ParserTest, parseLoopTest) {
  std::string test =
      R"(loop(0:10:2):
    //tralalal
)";
  std::string answer = "RootToken\n LoopToken\n";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, parseFunctionTest) {
  std::string test =
      R"(integer function count(integer int1, integer int2):
    //tralalal
)";
  std::string answer =
      "RootToken\n IntegerToken function count(IntegerToken int1, IntegerToken "
      "int2, )\n";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, parseFunctionDefaultArgumentsTest) {
  std::string test =
      R"(integer function count(integer int1 = 1, integer int2 = 10):
    //tralalal
)";
  std::string answer =
      "RootToken\n IntegerToken function count(IntegerToken int1, IntegerToken "
      "int2, )\n";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentIntegerTest) {
  std::string test = R"(integer wiek = rokUrodzenia - obecnyRok)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  AdditiveOperatorToken
   IdentifierToken
   IdentifierToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentIdentifierLiteralTest) {
  std::string test = R"(integer wiek = 2021 - rokUrodzenia)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  AdditiveOperatorToken
   IntegerLiteralToken
   IdentifierToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentLiteralLiteralTest) {
  std::string test = R"(integer wiek = 2021 - 1999)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  AdditiveOperatorToken
   IntegerLiteralToken
   IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentLiteralLiteral2Test) {
  std::string test = R"(integer wiek = 2021 - 1999 - 2)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  AdditiveOperatorToken
   IntegerLiteralToken
   AdditiveOperatorToken
    IntegerLiteralToken
    IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentLiteralTest) {
  std::string test = R"(integer wiek = 21)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentDoubleTest) {
  std::string test = R"(integer wiek = 21.5)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  DoubleLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentIdentifierTest) {
  std::string test = R"(integer wiek = wiek)";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  IntegerToken
  IdentifierToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentMatrix1Test) {
  std::string test = R"(matrix[2][2] macierz = [[1,2,3,4,5,6+7]])";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  MatrixToken macierz
   MatrixLiteralToken
    IntegerLiteralToken
    IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  AdditiveOperatorToken
   IntegerLiteralToken
   IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, assignmentMatrix2Test) {
  std::string test = R"(matrix[2][2+2] macierz = [[1,2,3,4,5,6+7]])";
  std::string answer =
      R"(RootToken
 AssignmentOperatorToken
  MatrixToken macierz
   MatrixLiteralToken
    IntegerLiteralToken
    AdditiveOperatorToken
     IntegerLiteralToken
     IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  IntegerLiteralToken
  AdditiveOperatorToken
   IntegerLiteralToken
   IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

