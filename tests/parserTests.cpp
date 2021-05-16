#include <gtest/gtest.h>
#include <iostream>
#include "parser/parser.hpp"

TEST(ParserTest, parseCommentTest) {
  StringSource src("//jestem komentarzem");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),Token::TokenType::CommentToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
}

TEST(ParserTest, parseLoopTest) {
  FileSource src("../tests/res/loop.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),Token::TokenType::RootToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  Node * statementNode = programNode->getChildren().front().get();
  EXPECT_EQ(statementNode->getChildren().size(), 14);
  EXPECT_EQ(statementNode->getChildren().back()->getToken().getType(), Token::TokenType::EndOfFileToken);
  std::vector<NodeUptr> const& statementChildren = statementNode->getChildren();
  EXPECT_EQ(statementChildren[0]->getToken().getType(), Token::TokenType::LoopToken);
  EXPECT_EQ(statementChildren[1]->getToken().getType(), Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(statementChildren[2]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[3]->getToken().getType(), Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[4]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[5]->getToken().getType(), Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[6]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[7]->getToken().getType(), Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(statementChildren[8]->getToken().getType(), Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[9]->getToken().getType(), Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[10]->getToken().getType(), Token::TokenType::OpenBlockToken);
  EXPECT_EQ(statementChildren[11]->getToken().getType(), Token::TokenType::CommentToken);
  EXPECT_EQ(statementChildren[12]->getToken().getType(), Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[13]->getToken().getType(), Token::TokenType::EndOfFileToken);
}

TEST(ParserTest, parseFunction) {
  FileSource src("../tests/res/function.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),Token::TokenType::RootToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  Node * statementNode = programNode->getChildren().front().get();
  EXPECT_EQ(statementNode->getChildren().size(), 14);
  EXPECT_EQ(statementNode->getChildren().back()->getToken().getType(), Token::TokenType::EndOfFileToken);
  std::vector<NodeUptr> const& statementChildren = statementNode->getChildren();
  EXPECT_EQ(statementChildren[0]->getToken().getType(), Token::TokenType::IntegerToken);
  EXPECT_EQ(statementChildren[1]->getToken().getType(), Token::TokenType::FunctionToken);
  EXPECT_EQ(statementChildren[2]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[3]->getToken().getType(), Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(statementChildren[4]->getToken().getType(), Token::TokenType::RootToken);
  EXPECT_EQ(statementChildren[5]->getToken().getType(), Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(statementChildren[6]->getToken().getType(), Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[7]->getToken().getType(), Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[8]->getToken().getType(), Token::TokenType::OpenBlockToken);
  EXPECT_EQ(statementChildren[9]->getToken().getType(), Token::TokenType::CommentToken);
  EXPECT_EQ(statementChildren[10]->getToken().getType(), Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[11]->getToken().getType(), Token::TokenType::ReturnToken);
  EXPECT_EQ(statementChildren[12]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[13]->getToken().getType(), Token::TokenType::EndOfFileToken);

}