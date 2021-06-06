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
      "RootToken\n IntegerToken function count\nIntegerToken int1 IntegerToken int2 ";
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
      "RootToken\n IntegerToken function count\nIntegerToken int1 IntegerToken "
      "int2 ";
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
  std::string test = R"(matrix[2][2] macierz = [1,2,3,4,5,6+7])";
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
  std::string test = R"(matrix[2][2+2] macierz = [1,2,3,4,5,6+7])";
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

TEST(ParserTest, aslasTest) {
  std::string test = R"(asLongAs(counter > 0):
  )";
  std::string answer =
      R"(RootToken
 AsLongAsToken
  LogicalOperatorToken
   IdentifierToken
   IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, aslas2Test) {
  std::string test = R"(asLongAs(counter > 0 and counter < 10):
  )";
  std::string answer =
      R"(RootToken
 AsLongAsToken
  AndToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, ifTest) {
  std::string test = R"(if(age < 10 or age > 10):
  price = 25.50
)";
  std::string answer =
      R"(RootToken
 IfToken
  OrToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
   AssignmentOperatorToken
    IdentifierToken
    DoubleLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, ifOtherwiseTest) {
  std::string test = R"(if(age < 10 or age > 10):
  price = 25.50
otherwise:
  price = 30.0
)";
  std::string answer =
      R"(RootToken
 IfToken
  OrToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
   LogicalOperatorToken
    IdentifierToken
    IntegerLiteralToken
   AssignmentOperatorToken
    IdentifierToken
    DoubleLiteralToken
  OtherwiseToken
   AssignmentOperatorToken
    IdentifierToken
    DoubleLiteralToken
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, conditionCaseTest1) {
  std::string test = R"(condition:
  case(age < 10):
    print('Child')
)";
  std::string answer =
      "RootToken\n ConditionToken\n  CaseToken\n   FunctionToken print\n    "
      "StringLiteralToken\n ";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}

TEST(ParserTest, condsitionCaseTest2) {
  std::string test = R"(condition:
    case(age < 10):
        print('Child')
    case(age >= 10 and age < 20):
        print('Teenager')
    case(age >= 20 and age < 60):
        print('Adult')
    case(age >= 60):
        print('Senior')
    default: // tutaj wpadamy gdy nic nie pasuje
        print('Error')
)";
  std::string answer =
      R"(RootToken
 ConditionToken
  CaseToken
   FunctionToken print
    StringLiteralToken
   CaseToken
   FunctionToken print
    StringLiteralToken
   CaseToken
   FunctionToken print
    StringLiteralToken
   CaseToken
   FunctionToken print
    StringLiteralToken
   DefaultToken
   FunctionToken print
    StringLiteralToken
 )";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  EXPECT_EQ(parser.getProgramNode()->getChildren().size(), 1);
  EXPECT_EQ(parser.getProgramNode()->getPrintTree(), answer);
}