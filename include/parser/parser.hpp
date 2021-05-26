#pragma once
#include <initializer_list>

#include "helpers/exception.hpp"
#include "lexicalAnalyzer/lexicalAnalyzer.hpp"
#include "lexicalAnalyzer/lexicalTable.hpp"
#include "parser/node.hpp"

class Parser {
 public:
  void parseProgram();
  NodeUptr parseExpression();
  void shiftToken();
  Parser(LexicalAnalyzer& newLexer)
      : programNode(std::make_unique<StatementNode>()),
        lexer(newLexer),
        currentToken(*lexer.getToken()) {}

  const StatementNode* getProgramNode() const {
    return static_cast<StatementNode*>(programNode.get());
  }

  std::string getProgramString();

 private:
  Token getToken();
  NodeUptr parseStatement();
  NodeUptr parseIfStatement();
  NodeUptr parseOtherwiseStatement();
  NodeUptr parseConditionStatement();
  NodeUptr parseLoopStatement();
  NodeUptr parseAsLAsStatement();
  NodeUptr parseFunctionStatement(NodeUptr root);
  NodeUptr parseCaseStatement();
  NodeUptr parseDefaultStatement();
  NodeUptr parseArguments();
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
  bool parseEndOfFile();
  bool accept(const Token::TokenType token);
  bool accept(std::initializer_list<Token::TokenType> list);
  bool expect(std::initializer_list<Token::TokenType> list);
  bool expect(const Token::TokenType token);
  NodeUptr programNode;
  LexicalAnalyzer lexer;
  Token currentToken;
};