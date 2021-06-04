#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

TEST(InterpreterTest, EvaluateExpressionTest) {
  std::string test =
      R"(loop(0:10:2):
    //tralalal
)";
  StringSource src(test);
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
}