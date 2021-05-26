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
