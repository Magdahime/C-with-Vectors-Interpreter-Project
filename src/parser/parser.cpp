#include "parser/parser.hpp"

Token Parser::getToken() {
  Token token = *lexer.getToken();
  while (token == Token::TokenType::CommentToken ||
         token == Token::TokenType::NextLineToken) {
    token = *lexer.getToken();
  }
  return token;
}

bool Parser::accept(const Token::TokenType token) {
  if (currentToken == token) {
    lastToken = currentToken;
    currentToken = getToken();
    return true;
  }
  return false;
}
bool Parser::accept(const std::vector<Token::TokenType>& acceptedTokens) {
  for (const auto& token : acceptedTokens) {
    if (currentToken == token) {
      lastToken = currentToken;
      currentToken = getToken();
      return true;
    }
  }
  return false;
}

bool Parser::expect(const Token::TokenType token) {
  if (accept(token)) return true;
  std::string message =
      "Token at " + currentToken.getLinePositionString() +
      " is unexpected. Expecting: " + LexicalTable::token2StringTable.at(token);
  throw UnexpectedToken(message);
  return false;
}

bool Parser::expect(const std::vector<Token::TokenType>& acceptedTokens) {
  if (accept(acceptedTokens)) return true;
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting: ";
  for (auto const& token : acceptedTokens) {
    message += LexicalTable::token2StringTable.at(token) + ",";
  }
  throw UnexpectedToken(message);
  return false;
}

void Parser::parseProgram() {
  NodeUptr node;
  while (node = parseStatement()) {
    programNode->add(std::move(node));
  }
  expect(Token::TokenType::EndOfFileToken);
}

NodeUptr Parser::parseStatement() {
  NodeUptr node;
  node = parseIfStatement();
  if (node) {
    return node;
  }
  node = parseConditionStatement();
  if (node) {
    return node;
  }
  node = parseLoopStatement();
  if (node) {
    return node;
  }
  node = parseAsLAsStatement();
  if (node) {
    return node;
  }
  node = parseFunCallOrAssignment();
  if (node) {
    return node;
  }
  node = parseFunStatOrAssignment();
  if (node) {
    return node;
  }
  return NodeUptr{};
}
//’if’, parentheses expr , ’:’, indent ,{statement} [other statement];
NodeUptr Parser::parseIfStatement() {
  if (accept(Token::TokenType::IfToken)) {
    NodeUptr ifNode = std::make_unique<RootNode>();
    ifNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    NodeUptr expressionNode = parseMultipleTestExpressions();
    ifNode->add(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr ifStatementsNodes;
    while (ifStatementsNodes = parseStatement()) {
      ifNode->add(std::move(ifStatementsNodes));
    }
    expect(endOfStatementTokens);
    NodeUptr otherwiseNode = parseOtherwiseStatement();
    if (otherwiseNode) ifNode->add(std::move(otherwiseNode));
    expect(endOfStatementTokens);
    return ifNode;
  }
  return NodeUptr{};
}
// other statement ::= ’otherwise’, ’:’, indent, {statement};
NodeUptr Parser::parseOtherwiseStatement() {
  if (accept(Token::TokenType::OtherwiseToken)) {
    NodeUptr otherwiseNode = std::make_unique<RootNode>();
    otherwiseNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr otherwiseStatementsNodes;
    while (otherwiseStatementsNodes = parseStatement()) {
      otherwiseNode->add(std::move(otherwiseStatementsNodes));
    }
    expect(endOfStatementTokens);
    return otherwiseNode;
  }
  return NodeUptr{};
}
// cond statement ::= ’condition’, ’:’, indent, {case statement},[default
// statement];
NodeUptr Parser::parseConditionStatement() {
  if (accept(Token::TokenType::ConditionToken)) {
    NodeUptr conditionNode = std::make_unique<RootNode>();
    conditionNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseCaseStatement()) {
      conditionNode->add(std::move(caseStatementsNodes));
    }
    NodeUptr defaultStatement = parseDefaultStatement();
    if (defaultStatement) conditionNode->add(std::move(defaultStatement));
    expect(endOfStatementTokens);
    return conditionNode;
  }
  return NodeUptr{};
}
// case statement ::= ’case’, parentheses expr, ’:’, indent, {statement};
NodeUptr Parser::parseCaseStatement() {
  if (accept(Token::TokenType::CaseToken)) {
    NodeUptr caseNode = std::make_unique<RootNode>();
    caseNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    NodeUptr expressionNode = parseMultipleTestExpressions();
    caseNode->add(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseStatement()) {
      caseNode->add(std::move(caseStatementsNodes));
    }
    expect(endOfStatementTokens);
    return caseNode;
  }
  return NodeUptr{};
}

// default statement ::= ’default’, ’:’, indent, { statement };
NodeUptr Parser::parseDefaultStatement() {
  if (accept(Token::TokenType::DefaultToken)) {
    NodeUptr defaultNode = std::make_unique<RootNode>();
    defaultNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr defaultStatementsNodes;
    while (defaultStatementsNodes = parseStatement()) {
      defaultNode->add(std::move(defaultStatementsNodes));
    }
    expect(endOfStatementTokens);
    return defaultNode;
  }
  return NodeUptr{};
}

// loop statement ::= ’loop’, ’(’, digit | identifier, ’:’, digit | identifier,
// [’:’,non zero digit | identifier]’)’, ’:’, indent, {statement};

NodeUptr Parser::parseLoopStatement() {
  if (accept(Token::TokenType::LoopToken)) {
    NodeUptr loopNode = std::make_unique<RootNode>();
    loopNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message);
    }
    loopNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message);
    }
    loopNode->add(std::make_unique<LiteralNode>(lastToken));
    if (accept(Token::TokenType::ColonToken)) {
      if (!(accept(Token::TokenType::IntegerLiteralToken) ||
            accept(Token::TokenType::IdentifierToken))) {
        std::string message =
            "Token at " + currentToken.getLinePositionString() +
            " is unexpected. Expecting: identifier or integerLiteral token";
        throw UnexpectedToken(message);
      }
      loopNode->add(std::make_unique<LiteralNode>(lastToken));
    }
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr loopStatementsNode;
    while (loopStatementsNode = parseStatement()) {
      loopNode->add(std::move(loopStatementsNode));
    }
    expect(endOfStatementTokens);
    return loopNode;
  }
  return NodeUptr{};
}
// aslas statement ::= ’asLongAs’, parentheses expr, ’:’, indent, {statement};
NodeUptr Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    NodeUptr asLasNode = std::make_unique<RootNode>();
    asLasNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    NodeUptr expressionNode = parseMultipleTestExpressions();
    asLasNode->add(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr asLasStatementsNode;
    while (asLasStatementsNode = parseStatement()) {
      asLasNode->add(std::move(asLasStatementsNode));
    }

    expect(endOfStatementTokens);
    return asLasNode;
  }
  return NodeUptr{};
}
// fun statement ::= type, ’function’, identifier, ’(’,arguments,’)’,’:’,
// indent, {statement};
NodeUptr Parser::parseFunctionStatement(NodeUptr root) {
  if (accept(Token::TokenType::IdentifierToken)) {
    root->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    NodeUptr functionArgumentsNode = parseArguments();
    if (functionArgumentsNode) root->add(std::move(functionArgumentsNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    NodeUptr functionStatementsNode;
    while (functionStatementsNode = parseStatement()) {
      root->add(std::move(functionStatementsNode));
    }
    NodeUptr functionReturnStatementsNode = parseReturnStatement();
    if (functionReturnStatementsNode)
      root->add(functionReturnStatementsNode->getChildren());
    expect(endOfStatementTokens);
    return root;
  }
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting : Function statement";
  throw UnexpectedToken(message);
  return NodeUptr{};
}
//[return statement] ::= 'return', identifier | expression | number | matrix |
// string;
NodeUptr Parser::parseReturnStatement() {
  NodeUptr returnStatementNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::ReturnToken)) {
    if (!(accept(literalsTokens) ||
          accept(Token::TokenType::IdentifierToken))) {
      NodeUptr node = parseMatrixValue();
      if (node) return node;
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Expecting: return value.";
      throw UnexpectedToken(message);
    }
    returnStatementNode->add(std::make_unique<LiteralNode>(lastToken));
    return returnStatementNode;
  }
  return NodeUptr{};
}

// arguments ::= type, identifier [’=’, ’value’] {’,’, type, identifier [’=’,
// ’value’]};
NodeUptr Parser::parseArguments() {
  NodeUptr argumentsNode = std::make_unique<RootNode>();
  while (accept(typeTokens)) {
    expect(Token::TokenType::IdentifierToken);
    argumentsNode->add(std::make_unique<LiteralNode>(lastToken));
    NodeUptr defaultArgument = parseDefaultArgument();
    if (defaultArgument) {
      argumentsNode->add(defaultArgument->getChildren());
    }
    if (!accept(Token::TokenType::CommaToken)) break;
  }
  if (!argumentsNode->empty()) return argumentsNode;
  return NodeUptr{};
}

//[arguments] ::= type, identifier ['=', 'value'] \{',', type, identifier ['=',
//'value']\};
NodeUptr Parser::parseDefaultArgument() {
  if (accept(Token::TokenType::AssignmentOperatorToken)) {
    NodeUptr argumentsNode = std::make_unique<RootNode>();
    if (!accept(literalsTokens)) {
      NodeUptr node = parseMatrixValue();
      if (node)
        argumentsNode->add(std::move(node));
      else {
        std::string message = "Unexpected token at " +
                              currentToken.getLinePositionString() +
                              ". Expecting: literal or identifier.";
        throw UnexpectedToken(message);
      }

    } else {
      argumentsNode->add(std::make_unique<LiteralNode>(lastToken));
    }
    return argumentsNode;
  }
  return NodeUptr{};
}

NodeUptr Parser::parseFunCallOrAssignment() {
  if (accept(Token::TokenType::IdentifierToken)) {
    NodeUptr rootNode = std::make_unique<RootNode>();
    rootNode->add(std::make_unique<LiteralNode>(lastToken));
    if (accept(Token::TokenType::OpenRoundBracketToken)) {
      return parseFunCall(std::move(rootNode));
    } else if (accept(Token::TokenType::AssignmentOperatorToken)) {
      NodeUptr assignmentNode =
          std::make_unique<AssigmentOperatorNode>(lastToken);
      assignmentNode->add(rootNode->getChildren());
      return parseAssignment(std::move(assignmentNode));
    } else {
      std::string message =
          "Unexpected token at " + currentToken.getLinePositionString();
      throw UnexpectedToken(message);
    }
  }
  return NodeUptr{};
}

NodeUptr Parser::parseFunCall(NodeUptr root) {
  NodeUptr arguments = parseFunCallArguments();
  root->add(std::move(arguments));
  expect(Token::TokenType::CloseRoundBracketToken);
  return root;
}

NodeUptr Parser::parseFunCallArguments() {
  NodeUptr arguments = std::make_unique<RootNode>();
  while (accept(assignableTokens)) {
    arguments->add(std::make_unique<LiteralNode>(lastToken));
    accept(Token::TokenType::CommaToken);
  }
  return arguments;
}

NodeUptr Parser::parseAssignment(NodeUptr root) {
  NodeUptr expressionNode = parseExpression();
  root->add(std::move(expressionNode));
  return root;
}

// Sfaktoryzowane
//[fun statement] ::= type, 'function', identifier, '(',arguments,')',':',
// indent, \{statement\},[return statement] ; [assign expression] ::=
// identifier,
//'=' , expression; [matrix] ::= 'matrix',\{'[', non zero integer ,']'\},
// identifier, '=', matrix value;
NodeUptr Parser::parseFunStatOrAssignment() {
  NodeUptr funOrAs = std::make_unique<RootNode>();
  if (accept(typeTokens)) {
    funOrAs->add(std::make_unique<LiteralNode>(lastToken));
    if (accept(Token::TokenType::FunctionToken)) {
      funOrAs->add(std::make_unique<StatementNode>(lastToken));
      return parseFunctionStatement(std::move(funOrAs));
    } else if (NodeUptr matrixSize = parseMatrixSize()) {
      funOrAs->add(std::move(matrixSize));
      return parseMatrixAssignment(std::move(funOrAs));
    } else {
      return parseAssignExpression(std::move(funOrAs));
    }
  }
  return NodeUptr{};
}

//[assign expression] ::= identifier, '=' , expression;
NodeUptr Parser::parseAssignExpression(NodeUptr root) {
  if (expect(Token::TokenType::IdentifierToken)) {
    root->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::AssignmentOperatorToken);
    NodeUptr assignmentNode =
        std::make_unique<AssigmentOperatorNode>(lastToken);
    assignmentNode->add(root->getChildren());
    NodeUptr expressionNode = parseExpression();
    assignmentNode->add(std::move(expressionNode));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Assignment expression";
  throw UnexpectedToken(message);
  return NodeUptr{};
}

// matrix ::= ’matrix’,{’[’, non zero integer ,’]’}, identifier, ’=’,
// matrixvalue;
NodeUptr Parser::parseMatrixAssignment(NodeUptr matrixNode) {
  if (expect(Token::TokenType::IdentifierToken)) {
    matrixNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::AssignmentOperatorToken);
    NodeUptr assignmentNode =
        std::make_unique<AssigmentOperatorNode>(lastToken);
    assignmentNode->add(matrixNode->getChildren());
    NodeUptr matrixValues;
    matrixValues = parseMatrixValue();
    if (matrixValues->empty()) {
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Matrix values are missing!";
      throw UnexpectedToken(message);
    }
    assignmentNode->add(std::move(matrixValues));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Matrix assignment expression";
  throw UnexpectedToken(message);
  return NodeUptr{};
}

// matrix value ::= [,{’[’,{number},’]’},]
NodeUptr Parser::parseMatrixValue() {
  NodeUptr matrixValues = std::make_unique<RootNode>();
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      while (accept(Token::TokenType::IntegerLiteralToken) ||
             accept(Token::TokenType::DoubleToken)) {
        matrixValues->add(std::make_unique<LiteralNode>(lastToken));
        accept(Token::TokenType::CommaToken);
      }
      expect(Token::TokenType::CloseSquareBracketToken);
    }
    expect(Token::TokenType::CloseSquareBracketToken);
    return matrixValues;
  }
  return NodeUptr{};
}
//,{’[’, non zero integer ,’]’},
NodeUptr Parser::parseMatrixSize() {
  NodeUptr matrixSizeNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    expect(Token::TokenType::IntegerLiteralToken);
    matrixSizeNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::CloseSquareBracketToken);
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      expect(Token::TokenType::IntegerLiteralToken);
      matrixSizeNode->add(std::make_unique<LiteralNode>(lastToken));
      expect(Token::TokenType::CloseSquareBracketToken);
    }
    return matrixSizeNode;
  }
  return NodeUptr{};
}

// sum ::= term | sum, {(’-’|’+’) sum}
NodeUptr Parser::parseExpression() {
  NodeUptr leftSide = parseTerm();
  NodeUptr additiveOperatorNode;
  while (accept(Token::TokenType::AdditiveOperatorToken)) {
    additiveOperatorNode = std::make_unique<AdditiveOperatorNode>(lastToken);
    NodeUptr rightSide = parseTerm();
    additiveOperatorNode->add(std::move(leftSide));
    additiveOperatorNode->add(std::move(rightSide));
  }
  if (!additiveOperatorNode) return leftSide;
  return additiveOperatorNode;
}

// term ::= factor, {(’*’| ’/’), factor}
NodeUptr Parser::parseTerm() {
  NodeUptr leftSide = parseFactor();
  NodeUptr multiplicativeOperatorNode;
  while (accept(Token::TokenType::MultiplicativeOperatorToken)) {
    multiplicativeOperatorNode =
        std::make_unique<MultiplicativeOperatorNode>(lastToken);
    NodeUptr rightSide = parseFactor();
    multiplicativeOperatorNode->add(std::move(leftSide));
    multiplicativeOperatorNode->add(std::move(rightSide));
  }
  if (!multiplicativeOperatorNode) return leftSide;
  return multiplicativeOperatorNode;
}
// factor ::= base { [’ˆ’,exponent] }
NodeUptr Parser::parseFactor() {
  NodeUptr leftSide = parseParenthesesExpression();
  NodeUptr exponentiationOperatorNode;
  while (accept(Token::TokenType::ExponentiationOperatorToken)) {
    exponentiationOperatorNode =
        std::make_unique<ExponentiationOperatorNode>(lastToken);
    NodeUptr rightSide = parseFactor();
    exponentiationOperatorNode->add(std::move(leftSide));
    exponentiationOperatorNode->add(std::move(rightSide));
  }
  if (!exponentiationOperatorNode) return leftSide;
  return exponentiationOperatorNode;
}

// base ::= ’(’ sum ’)’ | identifier | number
NodeUptr Parser::parseParenthesesExpression() {
  if (accept(Token::TokenType::IdentifierToken)) {
    NodeUptr identifierNode = std::make_unique<LiteralNode>(lastToken);
    return identifierNode;
  } else if (accept(Token::TokenType::IntegerLiteralToken)) {
    NodeUptr integerNode = std::make_unique<LiteralNode>(lastToken);
    return integerNode;
  } else if (accept(Token::TokenType::DoubleLiteralToken)) {
    NodeUptr doubleNode = std::make_unique<LiteralNode>(lastToken);
    return doubleNode;
  } else if (accept(Token::TokenType::OpenRoundBracketToken)) {
    NodeUptr expressionNode = parseExpression();
    expect(Token::TokenType::CloseRoundBracketToken);
    return expressionNode;
  } else if (accept(Token::TokenType::AdditiveOperatorToken)) {
    NodeUptr additiveOperatorNode =
        std::make_unique<AdditiveOperatorNode>(lastToken);
    NodeUptr factorNode = parseFactor();
    additiveOperatorNode->add(std::move(parseFactor()));
    return additiveOperatorNode;
  } else {
    std::string message = "Unexpected token at " +
                          currentToken.getLinePositionString() +
                          ". Expecting: Expression";
    throw UnexpectedToken(message);
  }
  return NodeUptr{};
}

NodeUptr Parser::parseTestExpression() {
  NodeUptr leftHandExpression = parseExpression();
  NodeUptr testNode;
  while (accept(Token::TokenType::LogicalOperatorToken)) {
    testNode = std::make_unique<LogicalOperatorNode>(lastToken);
    NodeUptr rightHandExpression = parseExpression();
    testNode->add(std::move(leftHandExpression));
    testNode->add(std::move(rightHandExpression));
  }
  if (!testNode) return leftHandExpression;
  return testNode;
}

NodeUptr Parser::parseMultipleTestExpressions() {
  NodeUptr leftHandExpression = parseTestExpression();
  NodeUptr testNode;
  while (accept(connectorOfStatementTokens)) {
    testNode = std::make_unique<LogicalOperatorNode>(lastToken);
    NodeUptr rightHandExpression = parseTestExpression();
    testNode->add(std::move(leftHandExpression));
    testNode->add(std::move(rightHandExpression));
  }
  if (!testNode) return leftHandExpression;
  return testNode;
}