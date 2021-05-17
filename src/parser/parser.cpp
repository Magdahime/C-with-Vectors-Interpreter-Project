#include "parser/parser.hpp"

bool Parser::accept(const Token::TokenType& token) {
  if (currentToken == token) {
    lastToken = currentToken;
    currentToken = *lexer.getToken();
    return true;
  }
  return false;
}
bool Parser::accept(const std::vector<Token::TokenType> acceptedTokens) {
  for (auto& token : acceptedTokens) {
    if (currentToken == token) {
      lastToken = currentToken;
      currentToken = *lexer.getToken();
      return true;
    }
  }
  return false;
}

bool Parser::expect(const Token::TokenType& token) {
  if (accept(token)) return true;
  std::string message =
      "Token at " + currentToken.getLinePositionString() +
      " is unexpected. Expecting: " + LexicalTable::token2StringTable.at(token);
  throw UnexpectedToken(message.c_str());
  return false;
}

bool Parser::expect(const std::vector<Token::TokenType> acceptedTokens) {
  if (accept(acceptedTokens)) return true;
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting: ";
  for (auto const& token : acceptedTokens) {
    message += LexicalTable::token2StringTable.at(token) + ",";
  }
  throw UnexpectedToken(message.c_str());
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
  std::cout << "PARSE STATEMENT" << std::endl;
  node = parseIfStatement();
  if (node) {
    std::cout << "PARSE IF STATEMENT" << std::endl;
    return node;
  }
  node = parseConditionStatement();
  if (node) {
    std::cout << "PARSE CONDITION STATEMENT" << std::endl;
    return node;
  }
  node = parseLoopStatement();
  if (node) {
    std::cout << "PARSE LOOP STATEMENT" << std::endl;
    return node;
  }
  node = parseAsLAsStatement();
  if (node) {
    std::cout << "PARSE AS LONG AS STATEMENT" << std::endl;
    return node;
  }
  node = parseFunCallOrAssignment();
  if (node) {
    std::cout << "PARSE FUN CALL OR ASSIGNMENT" << std::endl;
    return node;
  }
  node = parseFunStatOrAssignment();
  if (node) {
    std::cout << "PARSE FUNCTION STATEMENT" << std::endl;
    return node;
  }
  node = parseComment();
  if (node) {
    std::cout << "PARSE COMMENT" << std::endl;
    return node;
  }
  node = parseNewline();
  if (node) {
    std::cout << "NEWLINE" << std::endl;
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
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr expressionNode = parseMultipleTestExpressions();
    ifNode->add(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::NextLineToken);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr ifStatementsNodes;
    while (ifStatementsNodes = parseStatement()) {
      ifNode->add(std::move(ifStatementsNodes));
    }
    expect(endOfStatementTokens);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr otherwiseNode = parseOtherwiseStatement();
    if (otherwiseNode) ifNode->add(std::move(otherwiseNode));
    expect(endOfStatementTokens);
    ifNode->add(std::make_unique<LeafNode>(lastToken));
    return ifNode;
  }
  return NodeUptr{};
}
// other statement ::= ’otherwise’, ’:’, indent, {statement};
NodeUptr Parser::parseOtherwiseStatement() {
  std::cout << "OTHERIWSe" << std::endl;
  if (accept(Token::TokenType::OtherwiseToken)) {
    NodeUptr otherwiseNode = std::make_unique<RootNode>();
    otherwiseNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    otherwiseNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::NextLineToken);
    otherwiseNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    otherwiseNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr otherwiseStatementsNodes;
    while (otherwiseStatementsNodes = parseStatement()) {
      otherwiseNode->add(std::move(otherwiseStatementsNodes));
    }
    expect(endOfStatementTokens);
    otherwiseNode->add(std::make_unique<LeafNode>(lastToken));
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
    conditionNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    conditionNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseCaseStatement()) {
      conditionNode->add(std::move(caseStatementsNodes));
    }
    NodeUptr defaultStatement = parseDefaultStatement();
    if (defaultStatement) conditionNode->add(std::move(defaultStatement));
    expect(endOfStatementTokens);
    return conditionNode;
    conditionNode->add(std::make_unique<LeafNode>(lastToken));
    return conditionNode;
  }
  return NodeUptr{};
}
// case statement ::= ’case’, parentheses expr, ’:’, indent, {statement};
NodeUptr Parser::parseCaseStatement() {
  if (accept(Token::TokenType::CaseToken)) {
    NodeUptr caseNode = std::make_unique<RootNode>();
    caseNode->add(std::make_unique<StatementNode>(lastToken));
    NodeUptr expressionNode = parseMultipleTestExpressions();
    caseNode->add(std::move(expressionNode));
    expect(Token::TokenType::ColonToken);
    caseNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    caseNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseCaseStatement()) {
      caseNode->add(std::move(caseStatementsNodes));
    }
    expect(endOfStatementTokens);
    caseNode->add(std::make_unique<LeafNode>(lastToken));
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
    defaultNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    defaultNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr defaultStatementsNodes;
    while (defaultStatementsNodes = parseStatement()) {
      defaultNode->add(std::move(defaultStatementsNodes));
    }
    expect(endOfStatementTokens);
    defaultNode->add(std::make_unique<LeafNode>(lastToken));
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
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message.c_str());
    }
    loopNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message.c_str());
    }
    loopNode->add(std::make_unique<LiteralNode>(lastToken));
    if (accept(Token::TokenType::ColonToken)) {
      loopNode->add(std::make_unique<LeafNode>(lastToken));
      if (!(accept(Token::TokenType::IntegerLiteralToken) ||
            accept(Token::TokenType::IdentifierToken))) {
        std::string message =
            "Token at " + currentToken.getLinePositionString() +
            " is unexpected. Expecting: identifier or integerLiteral token";
        throw UnexpectedToken(message.c_str());
      }
      loopNode->add(std::make_unique<LiteralNode>(lastToken));
    }
    expect(Token::TokenType::CloseRoundBracketToken);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::NextLineToken);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr loopStatementsNode;
    while (loopStatementsNode = parseStatement()) {
      loopNode->add(std::move(loopStatementsNode));
    }
    expect(endOfStatementTokens);
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    return loopNode;
  }
  return NodeUptr{};
}
// aslas statement ::= ’asLongAs’, parentheses expr, ’:’, indent, {statement};
NodeUptr Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    NodeUptr asLasNode = std::make_unique<RootNode>();
    asLasNode->add(std::make_unique<StatementNode>(lastToken));
    NodeUptr expressionNode = parseMultipleTestExpressions();
    asLasNode->add(std::move(expressionNode));
    expect(Token::TokenType::ColonToken);
    asLasNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    asLasNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr asLasStatementsNode;
    while (asLasStatementsNode = parseStatement()) {
      asLasNode->add(std::move(asLasStatementsNode));
    }
    expect(endOfStatementTokens);
    asLasNode->add(std::make_unique<LeafNode>(lastToken));
  }
  return NodeUptr{};
}
// fun statement ::= type, ’function’, identifier, ’(’,arguments,’)’,’:’,
// indent, {statement};
NodeUptr Parser::parseFunctionStatement(NodeUptr root) {
  if (accept(Token::TokenType::IdentifierToken)) {
    root->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    root->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr functionArgumentsNode = parseArguments();
    if (functionArgumentsNode) root->add(std::move(functionArgumentsNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    root->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    root->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::NextLineToken);
    root->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    root->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr functionStatementsNode;
    while (functionStatementsNode = parseStatement()) {
      root->add(std::move(functionStatementsNode));
    }
    NodeUptr functionReturnStatementsNode = parseReturnStatement();
    if (functionReturnStatementsNode)
      root->add(functionReturnStatementsNode->getChildren());
    expect(endOfStatementTokens);
    root->add(std::make_unique<LeafNode>(lastToken));
    return root;
  }
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting : Function statement";
  throw UnexpectedToken(message.c_str());
  return NodeUptr{};
}
//[return statement] ::= 'return', identifier | expression | number | matrix |
// string;
NodeUptr Parser::parseReturnStatement() {
  NodeUptr returnStatementNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::ReturnToken)) {
    returnStatementNode->add(std::make_unique<LeafNode>(lastToken));
    if (!(accept(literalsTokens) ||
          accept(Token::TokenType::IdentifierToken))) {
      NodeUptr node = parseMatrixValue();
      if (node) return node;
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Expecting: return value.";
      throw UnexpectedToken(message.c_str());
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
    argumentsNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::IdentifierToken);
    argumentsNode->add(std::make_unique<LiteralNode>(lastToken));
    NodeUptr defaultArgument = parseDefaultArgument();
    if (defaultArgument) {
      argumentsNode->add(defaultArgument->getChildren());
    }
    if (!accept(Token::TokenType::CommaToken))
      break;
    else
      argumentsNode->add(std::make_unique<LeafNode>(lastToken));
  }
  if (!argumentsNode->empty()) return argumentsNode;
  return NodeUptr{};
}

//[arguments] ::= type, identifier ['=', 'value'] \{',', type, identifier ['=',
//'value']\};
NodeUptr Parser::parseDefaultArgument() {
  if (accept(Token::TokenType::AssignmentOperatorToken)) {
    NodeUptr argumentsNode = std::make_unique<RootNode>();
    argumentsNode->add(std::make_unique<LeafNode>(lastToken));
    if (!accept(literalsTokens)) {
      NodeUptr node = parseMatrixValue();
      if (node)
        argumentsNode->add(std::move(node));
      else {
        std::string message = "Unexpected token at " +
                              currentToken.getLinePositionString() +
                              ". Expecting: literal or identifier.";
        throw UnexpectedToken(message.c_str());
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
      rootNode->add(std::make_unique<LeafNode>(lastToken));
      return parseFunCall(std::move(rootNode));
    } else if (accept(Token::TokenType::AssignmentOperatorToken)) {
      NodeUptr assignmentNode =
          std::make_unique<AssigmentOperatorNode>(lastToken);
      assignmentNode->add(rootNode->getChildren());
      return parseAssignment(std::move(assignmentNode));
    } else {
      std::string message =
          "Unexpected token at " + currentToken.getLinePositionString();
      throw UnexpectedToken(message.c_str());
    }
  }
  return NodeUptr{};
}

NodeUptr Parser::parseFunCall(NodeUptr root) {
  NodeUptr arguments = parseFunCallArguments();
  root->add(std::move(arguments));
  expect(Token::TokenType::CloseRoundBracketToken);
  root->add(std::make_unique<LeafNode>(lastToken));
  return root;
}

NodeUptr Parser::parseFunCallArguments() {
  NodeUptr arguments = std::make_unique<RootNode>();
  while (accept(assignableTokens)) {
    arguments->add(std::make_unique<LiteralNode>(lastToken));
    if (accept(Token::TokenType::CommaToken))
      arguments->add(std::make_unique<LeafNode>(lastToken));
  }
  return arguments;
}

NodeUptr Parser::parseAssignment(NodeUptr root) {
  expect(assignableTokens);
  root->add(std::make_unique<LiteralNode>(lastToken));
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
    expect(assignableTokens);
    assignmentNode->add(std::make_unique<LiteralNode>(lastToken));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Assignment expression";
  throw UnexpectedToken(message.c_str());
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
      throw UnexpectedToken(message.c_str());
    }
    assignmentNode->add(std::move(matrixValues));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Matrix assignment expression";
  throw UnexpectedToken(message.c_str());
  return NodeUptr{};
}

// matrix value ::= [,{’[’,{number},’]’},]
NodeUptr Parser::parseMatrixValue() {
  NodeUptr matrixValues = std::make_unique<RootNode>();
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    matrixValues->add(std::make_unique<LeafNode>(lastToken));
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      matrixValues->add(std::make_unique<LeafNode>(lastToken));
      while (accept(Token::TokenType::IntegerLiteralToken) ||
             accept(Token::TokenType::DoubleToken)) {
        matrixValues->add(std::make_unique<LiteralNode>(lastToken));
        if (accept(Token::TokenType::CommaToken))
          matrixValues->add(std::make_unique<LeafNode>(lastToken));
      }
      expect(Token::TokenType::CloseSquareBracketToken);
      matrixValues->add(std::make_unique<LeafNode>(lastToken));
    }
    expect(Token::TokenType::CloseSquareBracketToken);
    matrixValues->add(std::make_unique<LeafNode>(lastToken));
    return matrixValues;
  }
  return NodeUptr{};
}
//,{’[’, non zero integer ,’]’},
NodeUptr Parser::parseMatrixSize() {
  NodeUptr matrixSizeNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    matrixSizeNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::IntegerLiteralToken);
    matrixSizeNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::CloseSquareBracketToken);
    matrixSizeNode->add(std::make_unique<LeafNode>(lastToken));
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      matrixSizeNode->add(std::make_unique<LeafNode>(lastToken));
      expect(Token::TokenType::IntegerLiteralToken);
      matrixSizeNode->add(std::make_unique<LiteralNode>(lastToken));
      expect(Token::TokenType::CloseSquareBracketToken);
      matrixSizeNode->add(std::make_unique<LeafNode>(lastToken));
    }
    return matrixSizeNode;
  }
  return NodeUptr{};
}

//  comment ::= comment delimiter, visible char, newline;
//  comment delimiter ::= ’\\’ | "#";
NodeUptr Parser::parseComment() {
  if (accept(Token::TokenType::CommentToken)) {
    return std::make_unique<CommentNode>(lastToken);
  }
  return NodeUptr{};
}
//\n
NodeUptr Parser::parseNewline() {
  if (accept(Token::TokenType::NextLineToken)) {
    return std::make_unique<CommentNode>(lastToken);
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
  if(!additiveOperatorNode) return leftSide;
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
  if(! multiplicativeOperatorNode) return leftSide;
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
  if(!exponentiationOperatorNode) return leftSide;
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
    throw UnexpectedToken(message.c_str());
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
  if(!testNode) return leftHandExpression;
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
  if(!testNode) return leftHandExpression;
  return testNode;
}