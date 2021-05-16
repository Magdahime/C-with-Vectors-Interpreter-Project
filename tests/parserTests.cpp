#include <gtest/gtest.h>

#include <iostream>

#include "parser/parser.hpp"

TEST(ParserTest, parseCommentTest) {
  StringSource src("//jestem komentarzem");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::CommentToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
}

TEST(ParserTest, parseLoopTest) {
  FileSource src("../tests/res/loop.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  Node* statementNode = programNode->getChildren().front().get();
  EXPECT_EQ(statementNode->getChildren().size(), 14);
  EXPECT_EQ(statementNode->getChildren().back()->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  std::vector<NodeUptr> const& statementChildren = statementNode->getChildren();
  EXPECT_EQ(statementChildren[0]->getToken().getType(),
            Token::TokenType::LoopToken);
  EXPECT_EQ(statementChildren[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(statementChildren[2]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[3]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[4]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[5]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[6]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[7]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(statementChildren[8]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[9]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[10]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(statementChildren[11]->getToken().getType(),
            Token::TokenType::CommentToken);
  EXPECT_EQ(statementChildren[12]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[13]->getToken().getType(),
            Token::TokenType::EndOfFileToken);
}

TEST(ParserTest, parseFunction) {
  FileSource src("../tests/res/function.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  Node* statementNode = programNode->getChildren().front().get();
  EXPECT_EQ(statementNode->getChildren().size(), 14);
  EXPECT_EQ(statementNode->getChildren().back()->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  std::vector<NodeUptr> const& statementChildren = statementNode->getChildren();
  EXPECT_EQ(statementChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(statementChildren[1]->getToken().getType(),
            Token::TokenType::FunctionToken);
  EXPECT_EQ(statementChildren[2]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[3]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(statementChildren[4]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(statementChildren[5]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(statementChildren[6]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(statementChildren[7]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[8]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(statementChildren[9]->getToken().getType(),
            Token::TokenType::CommentToken);
  EXPECT_EQ(statementChildren[10]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(statementChildren[11]->getToken().getType(),
            Token::TokenType::ReturnToken);
  EXPECT_EQ(statementChildren[12]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(statementChildren[13]->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  Node* argumentsNode = statementChildren[4].get();
  EXPECT_EQ(argumentsNode->getChildren().size(), 5);
  std::vector<NodeUptr> const& argumentChildren = argumentsNode->getChildren();
  EXPECT_EQ(argumentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(argumentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(argumentChildren[2]->getToken().getType(),
            Token::TokenType::CommaToken);
  EXPECT_EQ(argumentChildren[3]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(argumentChildren[4]->getToken().getType(),
            Token::TokenType::IdentifierToken);
}

TEST(ParserTest, parseMatrix) {
  FileSource src("../tests/res/matrix.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  std::vector<NodeUptr> const& nodes = (programNode->getChildren().front()->getChildren());
  EXPECT_EQ(nodes.size(), 4);
  EXPECT_EQ(nodes[0]->getToken().getType(),
            Token::TokenType::MatrixToken);
  EXPECT_EQ(nodes[1]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(nodes[2]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(nodes[3]->getToken().getType(),
            Token::TokenType::RootToken);
}

TEST(ParserTest, parseAssignments) {
  FileSource src("../tests/res/assignments.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(programNode->getChildren().size(), 5);
  std::vector<NodeUptr> const& nodes = programNode->getChildren();
  Node* firstAssignment = nodes[0].get();
  Node* secondAssignment = nodes[2].get();
  Node* thirdAssignment = nodes[4].get();
  EXPECT_EQ(firstAssignment->getChildren().size(), 3);
  EXPECT_EQ(firstAssignment->getToken(), Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& firstAssignmentChildren =
      firstAssignment->getChildren();
  EXPECT_EQ(firstAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(firstAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(firstAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(secondAssignment->getChildren().size(), 3);
  EXPECT_EQ(secondAssignment->getToken(), Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& secondAssignmentChildren =
      secondAssignment->getChildren();
  EXPECT_EQ(secondAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(secondAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(secondAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(thirdAssignment->getChildren().size(), 3);
  EXPECT_EQ(thirdAssignment->getToken(), Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& thirdAssignmentChildren =
      thirdAssignment->getChildren();
  EXPECT_EQ(thirdAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::DoubleToken);
  EXPECT_EQ(thirdAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(thirdAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::DoubleLiteralToken);
}