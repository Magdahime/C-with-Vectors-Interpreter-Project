#pragma once
#include "helpers/exception.hpp"
#include "lexicalAnalyzer/lexicalAnalyzer.hpp"
#include "lexicalAnalyzer/lexicalTable.hpp"
#include "parser/node.hpp"

class Parser {
 public:
  void parseProgram();
  NodeUptr parseStatement();
  NodeUptr parseNewline();
  NodeUptr parseIfStatement();
  NodeUptr parseOtherwiseStatement();
  NodeUptr parseConditionStatement();
  NodeUptr parseLoopStatement();
  NodeUptr parseAsLAsStatement();
  NodeUptr parseFunctionStatement(NodeUptr root);
  NodeUptr parseCaseStatement();
  NodeUptr parseDefaultStatement();
  NodeUptr parseArguments();
  NodeUptr parseExpression();
  NodeUptr parseFactor();
  NodeUptr parseTerm();
  NodeUptr parseFunCallArguments();
  NodeUptr parseDefaultArgument();
  NodeUptr parseFunCallOrAssignment();
  NodeUptr parseFunStatOrAssignment();
  NodeUptr parseReturnStatement();
  NodeUptr parseFunCall(NodeUptr root);
  NodeUptr parseAssignment(NodeUptr root);
  NodeUptr parseAssignExpression(NodeUptr root);
  NodeUptr parseParenthesesExpression();
  NodeUptr parseTestExpression();
  NodeUptr parseMultipleTestExpressions();
  NodeUptr parseMatrixAssignment(NodeUptr matrixNode);
  NodeUptr parseMatrixValue();
  NodeUptr parseMatrixSize();
  NodeUptr parseComment();

  Parser(LexicalAnalyzer& newLexer)
      : programNode(std::make_unique<RootNode>()),
        lexer(newLexer),
        currentToken(*lexer.getToken()),
        lastToken(currentToken) {}

  Node* getProgramNode() { return programNode.get(); }

 private:
  const std::vector<Token::TokenType> literalsTokens{
      Token::TokenType::IntegerLiteralToken,
      Token::TokenType::DoubleLiteralToken,
      Token::TokenType::StringLiteralToken};
  const std::vector<Token::TokenType> assignableTokens{
      Token::TokenType::IntegerLiteralToken,
      Token::TokenType::DoubleLiteralToken,
      Token::TokenType::StringLiteralToken, Token::TokenType::IdentifierToken};
  const std::vector<Token::TokenType> typeTokens{
      Token::TokenType::IntegerToken, Token::TokenType::DoubleToken,
      Token::TokenType::MatrixToken, Token::TokenType::TextToken};
  const std::vector<Token::TokenType> endOfStatementTokens{
      Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken};
  const std::vector<Token::TokenType> connectorOfStatementTokens{
      Token::TokenType::AndToken, Token::TokenType::OrToken,
      Token::TokenType::NotToken};
  bool accept(const Token::TokenType& token);
  bool accept(const std::vector<Token::TokenType> acceptedTokens);
  bool expect(const std::vector<Token::TokenType> acceptedTokens);
  bool expect(const Token::TokenType& token);
  NodeUptr programNode;
  LexicalAnalyzer lexer;
  Token currentToken;
  Token lastToken;
};