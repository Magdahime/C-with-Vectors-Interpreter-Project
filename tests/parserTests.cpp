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
  std::vector<NodeUptr> const& nodes =
      (programNode->getChildren().front()->getChildren());
  EXPECT_EQ(nodes.size(), 4);
  EXPECT_EQ(nodes[0]->getToken().getType(), Token::TokenType::MatrixToken);
  EXPECT_EQ(nodes[1]->getToken().getType(), Token::TokenType::RootToken);
  EXPECT_EQ(nodes[2]->getToken().getType(), Token::TokenType::IdentifierToken);
  EXPECT_EQ(nodes[3]->getToken().getType(), Token::TokenType::RootToken);
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
  EXPECT_EQ(firstAssignment->getToken(),
            Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& firstAssignmentChildren =
      firstAssignment->getChildren();
  EXPECT_EQ(firstAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(firstAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(firstAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(secondAssignment->getChildren().size(), 3);
  EXPECT_EQ(secondAssignment->getToken(),
            Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& secondAssignmentChildren =
      secondAssignment->getChildren();
  EXPECT_EQ(secondAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(secondAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(secondAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(thirdAssignment->getChildren().size(), 3);
  EXPECT_EQ(thirdAssignment->getToken(),
            Token::TokenType::AssignmentOperatorToken);
  std::vector<NodeUptr> const& thirdAssignmentChildren =
      thirdAssignment->getChildren();
  EXPECT_EQ(thirdAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::DoubleToken);
  EXPECT_EQ(thirdAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(thirdAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::DoubleLiteralToken);
}
TEST(ParserTest, parseFunCallOrAssignment) {
  FileSource src("../tests/res/funCallOrAssignment.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(programNode->getChildren().size(), 5);
  std::vector<NodeUptr> const& nodes = programNode->getChildren();
  Node* firstAssignment = nodes[0].get();
  Node* funcall = nodes[2].get();
  Node* secondAssignment = nodes[4].get();
  EXPECT_EQ(firstAssignment->getChildren().size(), 3);
  EXPECT_EQ(secondAssignment->getChildren().size(), 2);
  EXPECT_EQ(funcall->getChildren().size(), 4);
  std::vector<NodeUptr> const& funcallChildren = funcall->getChildren();
  EXPECT_EQ(funcallChildren[0]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(funcallChildren[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(funcallChildren[2]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(funcallChildren[3]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  std::vector<NodeUptr> const& secondAssignmentChildren =
      secondAssignment->getChildren();
  EXPECT_EQ(secondAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(secondAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  std::vector<NodeUptr> const& firstAssignmentChildren =
      firstAssignment->getChildren();
  EXPECT_EQ(firstAssignmentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerToken);
  EXPECT_EQ(firstAssignmentChildren[1]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(firstAssignmentChildren[2]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
}

TEST(ParserTest, ifOtherwiseTest) {
  FileSource src("../tests/res/if.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(programNode->getChildren().size(), 3);
  std::vector<NodeUptr> const& nodes = programNode->getChildren();
  Node* assignment = nodes[0].get();
  Node* ifNode = nodes[2].get();
  EXPECT_EQ(assignment->getChildren().size(), 3);
  EXPECT_EQ(ifNode->getChildren().size(), 12);
  std::vector<NodeUptr> const& ifNodeChildren = ifNode->getChildren();
  EXPECT_EQ(ifNodeChildren[0]->getToken().getType(), Token::TokenType::IfToken);
  EXPECT_EQ(ifNodeChildren[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(ifNodeChildren[2]->getToken().getType(),
            Token::TokenType::LogicalOperatorToken);
  EXPECT_EQ(ifNodeChildren[3]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(ifNodeChildren[4]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(ifNodeChildren[5]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(ifNodeChildren[6]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(ifNodeChildren[7]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(ifNodeChildren[8]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(ifNodeChildren[9]->getToken().getType(),
            Token::TokenType::CloseBlockToken);
  EXPECT_EQ(ifNodeChildren[10]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(ifNodeChildren[11]->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  EXPECT_EQ(ifNodeChildren[2]->getChildren().size(), 2);
  std::vector<NodeUptr> const& conditionChildren =
      ifNodeChildren[2]->getChildren();
  EXPECT_EQ(conditionChildren[0]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(conditionChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(ifNodeChildren[10]->getChildren().size(), 6);
  std::vector<NodeUptr> const& otherwiseChildren =
      ifNodeChildren[10]->getChildren();
  EXPECT_EQ(otherwiseChildren[0]->getToken().getType(),
            Token::TokenType::OtherwiseToken);
  EXPECT_EQ(otherwiseChildren[1]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(otherwiseChildren[2]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(otherwiseChildren[3]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(otherwiseChildren[4]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(otherwiseChildren[5]->getToken().getType(),
            Token::TokenType::EndOfFileToken);  // Tutaj jest wywołanie funkcji
  EXPECT_EQ(ifNodeChildren[7]->getChildren().size(), 4);
  std::vector<NodeUptr> const& ifChildren = ifNodeChildren[7]->getChildren();
  EXPECT_EQ(ifChildren[0]->getToken().getType(),
            Token::TokenType::IdentifierToken);
  EXPECT_EQ(ifChildren[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(ifChildren[2]->getToken().getType(),
            Token::TokenType::RootToken);  // Tutaj są argumenty
  EXPECT_EQ(ifChildren[3]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(ifChildren[2]->getChildren().size(), 1);
}

TEST(ParserTest, expressionTest1) {
  StringSource src("2+2*2");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  NodeUptr expressionNode = parser.parseExpression();
  EXPECT_EQ(expressionNode->getChildren().size(), 2);
  EXPECT_EQ(expressionNode->getToken().getType(),
            Token::TokenType::AdditiveOperatorToken);
  std::vector<NodeUptr> const& expressionChildren =
      expressionNode->getChildren();
  EXPECT_EQ(expressionChildren[0]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(expressionChildren[1]->getToken().getType(),
            Token::TokenType::MultiplicativeOperatorToken);
  EXPECT_EQ(expressionChildren[1]->getChildren().size(), 2);
  std::vector<NodeUptr> const& multiplicationChildren =
      expressionChildren[1]->getChildren();
  EXPECT_EQ(multiplicationChildren[0]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(multiplicationChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
}

TEST(ParserTest, expressionTest2) {
  StringSource src("(2+2)*2");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  NodeUptr expressionNode = parser.parseExpression();
  EXPECT_EQ(expressionNode->getChildren().size(), 2);
  EXPECT_EQ(expressionNode->getToken().getType(),
            Token::TokenType::MultiplicativeOperatorToken);
  std::vector<NodeUptr> const& expressionChildren =
      expressionNode->getChildren();
  EXPECT_EQ(expressionChildren[0]->getToken().getType(),
            Token::TokenType::AdditiveOperatorToken);
  EXPECT_EQ(expressionChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(expressionChildren[0]->getChildren().size(), 2);
  std::vector<NodeUptr> const& multiplicationChildren =
      expressionChildren[0]->getChildren();
  EXPECT_EQ(multiplicationChildren[0]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(multiplicationChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
}

TEST(ParserTest, expressionTest3) {
  StringSource src("(2+2)*2^2");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  NodeUptr expressionNode = parser.parseExpression();
  EXPECT_EQ(expressionNode->getChildren().size(), 2);
  EXPECT_EQ(expressionNode->getToken().getType(),
            Token::TokenType::MultiplicativeOperatorToken);
  std::vector<NodeUptr> const& expressionChildren =
      expressionNode->getChildren();
  EXPECT_EQ(expressionChildren[0]->getToken().getType(),
            Token::TokenType::AdditiveOperatorToken);
  EXPECT_EQ(expressionChildren[1]->getToken().getType(),
            Token::TokenType::ExponentiationOperatorToken);
  EXPECT_EQ(expressionChildren[1]->getChildren().size(), 2);
  EXPECT_EQ(expressionChildren[0]->getChildren().size(), 2);
  std::vector<NodeUptr> const& addChildren =
      expressionChildren[0]->getChildren();
  EXPECT_EQ(addChildren[0]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(addChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  std::vector<NodeUptr> const& exponentChildren =
      expressionChildren[1]->getChildren();
  EXPECT_EQ(exponentChildren[0]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
  EXPECT_EQ(exponentChildren[1]->getToken().getType(),
            Token::TokenType::IntegerLiteralToken);
}

TEST(ParserTest, aslasTest) {
  FileSource src("../tests/res/aslas.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().front()->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(programNode->getChildren().size(), 1);
  std::vector<NodeUptr> const& nodes = programNode->getChildren();
  Node* aslasStatement = nodes[0].get();
  EXPECT_EQ(aslasStatement->getChildren().size(), 11);
  std::vector<NodeUptr> const& aslasChildren = aslasStatement->getChildren();
  EXPECT_EQ(aslasChildren[0]->getToken().getType(),
            Token::TokenType::AsLongAsToken);
  EXPECT_EQ(aslasChildren[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(aslasChildren[2]->getToken().getType(),
            Token::TokenType::LogicalOperatorToken);
  EXPECT_EQ(aslasChildren[3]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(aslasChildren[4]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(aslasChildren[5]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(aslasChildren[6]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(aslasChildren[7]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(aslasChildren[8]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(aslasChildren[9]->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(aslasChildren[10]->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  EXPECT_EQ(aslasChildren[7]->getChildren().size(), 4);
}

TEST(ParserTest, conditionCaseTest) {
  FileSource src("../tests/res/condition.mpp");
  LexicalAnalyzer lexicAna(&src);
  Parser parser(lexicAna);
  parser.parseProgram();
  auto programNode = parser.getProgramNode();
  EXPECT_EQ(programNode->getChildren().size(), 5);
  std::vector<NodeUptr> const& conditionChildren = programNode->getChildren();
  EXPECT_EQ(conditionChildren[0]->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(conditionChildren[1]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(conditionChildren[2]->getToken().getType(),
            Token::TokenType::AssignmentOperatorToken);
  EXPECT_EQ(conditionChildren[3]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(conditionChildren[4]->getToken().getType(),
            Token::TokenType::RootToken);
  std::vector<NodeUptr> const& conditionCaseChildren =
      conditionChildren[4]->getChildren();
  EXPECT_EQ(conditionCaseChildren.size(), 10);
  EXPECT_EQ(conditionCaseChildren[0]->getToken().getType(),
            Token::TokenType::ConditionToken);
  EXPECT_EQ(conditionCaseChildren[1]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(conditionCaseChildren[2]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(conditionCaseChildren[3]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(conditionCaseChildren[4]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(conditionCaseChildren[5]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(conditionCaseChildren[6]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(conditionCaseChildren[7]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(conditionCaseChildren[8]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(conditionCaseChildren[9]->getToken().getType(),
            Token::TokenType::EndOfFileToken);
  EXPECT_EQ(conditionCaseChildren[4]->getChildren().size(), 10);
  EXPECT_EQ(conditionCaseChildren[5]->getChildren().size(), 10);
  EXPECT_EQ(conditionCaseChildren[6]->getChildren().size(), 10);
  EXPECT_EQ(conditionCaseChildren[7]->getChildren().size(), 10);
  EXPECT_EQ(conditionCaseChildren[8]->getChildren().size(), 8);
  std::vector<NodeUptr> const& case1Children =
      conditionCaseChildren[4]->getChildren();
  EXPECT_EQ(case1Children[0]->getToken().getType(),
            Token::TokenType::CaseToken);
  EXPECT_EQ(case1Children[1]->getToken().getType(),
            Token::TokenType::OpenRoundBracketToken);
  EXPECT_EQ(case1Children[2]->getToken().getType(),
            Token::TokenType::LogicalOperatorToken);
  EXPECT_EQ(case1Children[3]->getToken().getType(),
            Token::TokenType::CloseRoundBracketToken);
  EXPECT_EQ(case1Children[4]->getToken().getType(),
            Token::TokenType::ColonToken);
  EXPECT_EQ(case1Children[5]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(case1Children[6]->getToken().getType(),
            Token::TokenType::OpenBlockToken);
  EXPECT_EQ(case1Children[7]->getToken().getType(),
            Token::TokenType::RootToken);
  EXPECT_EQ(case1Children[8]->getToken().getType(),
            Token::TokenType::NextLineToken);
  EXPECT_EQ(case1Children[9]->getToken().getType(),
            Token::TokenType::CloseBlockToken);
}