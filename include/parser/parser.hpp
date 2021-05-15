#pragma once
#include "helpers/exception.hpp"
#include "lexicalAnalyzer/lexicalAnalyzer.hpp"
#include "lexicalAnalyzer/lexicalTable.hpp"
#include "parser/node.hpp"

class Parser {
 public:
  std::optional<NodeUptr> parseProgram();
  std::optional<NodeUptr> parseStatement();
  std::optional<NodeUptr> parseIfStatement();
  std::optional<NodeUptr> parseOtherwiseStatement();
  std::optional<NodeUptr> parseConditionStatement();
  std::optional<NodeUptr> parseLoopStatement();
  std::optional<NodeUptr> parseLoopStepStatement();
  std::optional<NodeUptr> parseAsLAsStatement();
  std::optional<NodeUptr> parseFunctionStatement();
  std::optional<NodeUptr> parseCaseStatement();
  std::optional<NodeUptr> parseDefaultStatement();
  std::optional<NodeUptr> parseArguments();
  std::optional<NodeUptr> parseDefaultArgument();
  std::optional<NodeUptr> parseFunCallOrAssignment();
  std::optional<NodeUptr> parseReturnStatement();
  std::optional<NodeUptr> parseAssignExpression();
  std::optional<NodeUptr> parseParenthesesExpression();
  std::optional<NodeUptr> parseTestExpression();
  std::optional<NodeUptr> parseMatrix();
  std::optional<NodeUptr> parseMatrixValue();
  std::optional<NodeUptr> parseMatrixSize();
  std::optional<NodeUptr> parseComment();

  Parser(LexicalAnalyzer& lexer)
      : lexer(lexer), currentToken(*lexer.getToken()) {}

 private:
  const std::vector<Token::TokenType> literalsTokens{
      Token::TokenType::IdentifierToken, Token::TokenType::IntegerLiteralToken,
      Token::TokenType::DoubleLiteralToken,
      Token::TokenType::StringLiteralToken};
  const std::vector<Token::TokenType> typeTokens{
      Token::TokenType::IntegerToken, Token::TokenType::DoubleToken,
      Token::TokenType::MatrixToken, Token::TokenType::TextToken };
  bool accept(const Token::TokenType& token);
  bool accept(const std::vector<Token::TokenType> acceptedTokens);
  bool expect(const Token::TokenType& token);
  NodeUptr programNode;
  LexicalAnalyzer lexer;
  Token currentToken;
};