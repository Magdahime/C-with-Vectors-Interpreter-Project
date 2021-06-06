#include "parser/parser.hpp"

Token Parser::getToken() {
  Token token = *lexer.getToken();
  while (token == Token::TokenType::CommentToken ||
         token == Token::TokenType::NextLineToken) {
    token = *lexer.getToken();
  }
  return token;
}

void Parser::shiftToken() { currentToken = getToken(); }

bool Parser::accept(const Token::TokenType token) {
  if (currentToken == token) return true;
  return false;
}
bool Parser::accept(std::initializer_list<Token::TokenType> list) {
  if (std::ranges::any_of(
          list, [&](const auto& token) { return token == currentToken; }))
    return true;
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

bool Parser::expect(std::initializer_list<Token::TokenType> list) {
  if (accept(list)) return true;
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting: ";
  for (auto const& token : list) {
    message += LexicalTable::token2StringTable.at(token) + ",";
  }
  throw UnexpectedToken(message);
  return false;
}

void Parser::parseProgram() {
  StatementNodeUptr node;
  while (node = parseStatement()) {
    programNode->add(std::move(node));
  }
  if (!parseEndOfFile()) {
    std::string message = "Token at " + currentToken.getLinePositionString() +
                          " is unexpected. Expecting: EOF.";
    throw UnexpectedToken(message);
  }
}

StatementNodeUptr Parser::parseStatement() {
  StatementNodeUptr node;
  if ((node = parseLoopStatement()) || (node = parseReturnStatement()) ||
      (node = parseFunStatOrAssignment()) || (node = parseAsLAsStatement()) ||
      (node = parseIfStatement()) || (node = parseFunCallOrAssignment()) ||
      (node = parseConditionStatement())) {
    return node;
  }
  return StatementNodeUptr{};
}

bool Parser::parseEndOfFile() {
  return expect(Token::TokenType::EndOfFileToken);
}

//’if’, parentheses expr , ’:’, indent ,{statement} [other statement];
StatementNodeUptr Parser::parseIfStatement() {
  if (accept(Token::TokenType::IfToken)) {
    IfStatementNodeUptr ifNode =
        std::make_unique<IfStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::OpenRoundBracketToken);
    shiftToken();
    ExpressionNodeUptr expressionNode = parseMultipleTestExpressions();
    ifNode->setIfExpression(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr ifStatementsNodes;
    while (ifStatementsNodes = parseStatement()) {
      ifNode->add(std::move(ifStatementsNodes));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    StatementNodeUptr otherwiseNode = parseOtherwiseStatement();
    if (otherwiseNode) ifNode->setOtherwiseExpression(std::move(otherwiseNode));
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return ifNode;
  }
  return StatementNodeUptr{};
}

// other statement ::= ’otherwise’, ’:’, indent, {statement};
StatementNodeUptr Parser::parseOtherwiseStatement() {
  if (accept(Token::TokenType::OtherwiseToken)) {
    OtherwiseStatementNodeUptr otherwiseNode =
        std::make_unique<OtherwiseStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr otherwiseStatementsNodes;
    while (otherwiseStatementsNodes = parseStatement()) {
      otherwiseNode->add(std::move(otherwiseStatementsNodes));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return otherwiseNode;
  }
  return StatementNodeUptr{};
}

// // cond statement ::= ’condition’, ’:’, indent, {case statement},[default
// // statement];
StatementNodeUptr Parser::parseConditionStatement() {
  if (accept(Token::TokenType::ConditionToken)) {
    ConditionStatementNodeUptr conditionNode =
        std::make_unique<ConditionStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseCaseStatement()) {
      conditionNode->add(std::move(caseStatementsNodes));
    }
    StatementNodeUptr defaultStatement = parseDefaultStatement();
    if (defaultStatement)
      conditionNode->setDefault(std::move(defaultStatement));
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return conditionNode;
  }
  return StatementNodeUptr{};
}
// case statement ::= ’case’, parentheses expr, ’:’, indent, {statement};
StatementNodeUptr Parser::parseCaseStatement() {
  if (accept(Token::TokenType::CaseToken)) {
    CaseStatementNodeUptr caseNode =
        std::make_unique<CaseStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::OpenRoundBracketToken);
    shiftToken();
    ExpressionNodeUptr expressionNode = parseMultipleTestExpressions();
    caseNode->setCaseExpression(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr caseStatementsNodes;
    while (caseStatementsNodes = parseStatement()) {
      caseNode->add(std::move(caseStatementsNodes));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return caseNode;
  }
  return StatementNodeUptr{};
}

// default statement ::= ’default’, ’:’, indent, { statement };
StatementNodeUptr Parser::parseDefaultStatement() {
  if (accept(Token::TokenType::DefaultToken)) {
    DefaultStatementNodeUptr defaultNode =
        std::make_unique<DefaultStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr defaultStatementsNodes;
    while (defaultStatementsNodes = parseStatement()) {
      defaultNode->add(std::move(defaultStatementsNodes));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return defaultNode;
  }
  return StatementNodeUptr{};
}

// loop statement ::= ’loop’, ’(’, digit | identifier, ’:’, digit |
// identifier,
// [’:’,non zero digit | identifier]’)’, ’:’, indent, {statement};
StatementNodeUptr Parser::parseLoopStatement() {
  if (accept(Token::TokenType::LoopToken)) {
    std::unique_ptr<LoopStatementNode> loopNode =
        std::make_unique<LoopStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::OpenRoundBracketToken);
    shiftToken();
    ExpressionNodeUptr start = parseExpression();
    if (!start)
      throw UnexpectedToken("Token at " + currentToken.getLinePositionString() +
                            " is unexpected. Expecting: start expression");
    loopNode->setStart(std::move(start));
    expect(Token::TokenType::ColonToken);
    shiftToken();
    ExpressionNodeUptr end = parseExpression();
    if (!end)
      throw UnexpectedToken(
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token");
    loopNode->setEnd(std::move(end));
    if (accept(Token::TokenType::ColonToken)) {
      shiftToken();
      ExpressionNodeUptr step = parseExpression();
      if (!step)
        throw UnexpectedToken(
            "Token at " + currentToken.getLinePositionString() +
            " is unexpected. Expecting: identifier or integerLiteral token");
      loopNode->setStep(std::move(step));
    }
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    // interpreter.enterBlock();
    shiftToken();
    StatementNodeUptr loopStatementsNode;
    while (loopStatementsNode = parseStatement()) {
      loopNode->add(std::move(loopStatementsNode));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    // loopNode->accept(interpreter);
    // interpreter.closeBlock();
    return loopNode;
  }
  return StatementNodeUptr{};
}

// aslas statement ::= ’asLongAs’, parentheses expr, ’:’, indent,
//{statement};
StatementNodeUptr Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    AslasStatementNodeUptr asLasNode =
        std::make_unique<AslasStatementNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::OpenRoundBracketToken);
    shiftToken();
    ExpressionNodeUptr expressionNode = parseMultipleTestExpressions();
    asLasNode->setAsLAsExpression(std::move(expressionNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr asLasStatementsNode;
    while (asLasStatementsNode = parseStatement()) {
      asLasNode->add(std::move(asLasStatementsNode));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return asLasNode;
  }
  return StatementNodeUptr{};
}

// fun statement ::= type, ’function’, identifier, ’(’,arguments,’)’,’:’,
// indent, {statement};

StatementNodeUptr Parser::parseFunctionStatement(
    FunctionStatementNodeUptr root) {
  if (accept(Token::TokenType::IdentifierToken)) {
    root->setIdentifier(currentToken.getString());
    shiftToken();
    expect(Token::TokenType::OpenRoundBracketToken);
    shiftToken();
    std::vector<ArgumentNodeUptr> arguments = parseArguments();
    if (arguments.size() != 0) root->setArgumentsNodes(arguments);
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr functionStatementsNode;
    while (functionStatementsNode = parseStatement()) {
      root->add(std::move(functionStatementsNode));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return root;
  }
  std::string message = "Token at " + currentToken.getLinePositionString() +
                        " is unexpected. Expecting : Function statement";
  throw UnexpectedToken(message);
  return StatementNodeUptr{};
}
//[return statement] ::= 'return', identifier | expression | number | matrix
//|string;
StatementNodeUptr Parser::parseReturnStatement() {
  if (accept(Token::TokenType::ReturnToken)) {
    ChildrenStatementNodeUptr returnStatementNode =
        std::make_unique<ChildrenStatementNode>(currentToken);
    shiftToken();
    if (!(accept({Token::TokenType::IntegerLiteralToken,
                  Token::TokenType::DoubleLiteralToken,
                  Token::TokenType::StringLiteralToken}))) {
      MatrixValueNodeUptr matrix = parseMatrixValue();
      if (matrix) return matrix;
      ExpressionNodeUptr expression = parseExpression();
      if (expression) return expression;
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Expecting: return value.";
      throw UnexpectedToken(message);
    }
    shiftToken();
    return returnStatementNode;
  }
  return StatementNodeUptr{};
}

// arguments ::= type, identifier [’=’, ’value’] {’,’, type, identifier [’=’,
// ’value’]};
std::vector<ArgumentNodeUptr> Parser::parseArguments() {
  std::vector<ArgumentNodeUptr> arguments;
  while (accept({Token::TokenType::IntegerToken, Token::TokenType::DoubleToken,
                 Token::TokenType::MatrixToken, Token::TokenType::TextToken})) {
    ArgumentNodeUptr argument = std::make_unique<ArgumentNode>(currentToken);
    shiftToken();
    expect(Token::TokenType::IdentifierToken);
    argument->setIdentifier(currentToken.getString());
    shiftToken();
    ExpressionNodeUptr value = parseDefaultArgument();
    if (value) argument->setDefaultValue(std::move(value));
    arguments.push_back(std::move(argument));
    if (!accept(Token::TokenType::CommaToken)) break;
    shiftToken();
  }
  if (!arguments.empty()) return arguments;
  return {};
}

//[arguments] ::= type, identifier ['=', 'value'] \{',', type, identifier
//['=','value']\};
ExpressionNodeUptr Parser::parseDefaultArgument() {
  if (accept(Token::TokenType::AssignmentOperatorToken)) {
    shiftToken();
    if (!accept({Token::TokenType::IntegerLiteralToken,
                 Token::TokenType::DoubleLiteralToken,
                 Token::TokenType::StringLiteralToken})) {
      MatrixValueNodeUptr matrix = parseMatrixValue();
      if (matrix) {
        return matrix;
        ExpressionNodeUptr expression = parseExpression();
        if (expression) return expression;
      } else {
        throw UnexpectedToken("Unexpected token at " +
                              currentToken.getLinePositionString() +
                              ". Expecting: literal or identifier.");
      }
    } else {
      ValueNodeUptr returnToken = std::make_unique<ValueNode>(currentToken);
      shiftToken();
      return returnToken;
    }
  }
  return {};
}

StatementNodeUptr Parser::parseFunCallOrAssignment() {
  if (accept(Token::TokenType::IdentifierToken)) {
    Token identifier = currentToken;
    shiftToken();
    if (accept(Token::TokenType::OpenRoundBracketToken)) {
      shiftToken();
      FunctionCallNodeUptr funcallNode = std::make_unique<FunctionCallNode>(
          Token(Token::TokenType::FunctionToken));
      funcallNode->setIdentifier(identifier.getString());
      return parseFunCall(std::move(funcallNode));
    } else if (accept(Token::TokenType::AssignmentOperatorToken)) {
      AssignNewValueNodeUptr assignmentNode =
          std::make_unique<AssignNewValueNode>(currentToken);
      shiftToken();
      ValueNodeUptr valueNode = std::make_unique<ValueNode>(identifier);
      assignmentNode->add(std::move(valueNode));
      return parseAssignment(std::move(assignmentNode));
    } else {
      std::string message =
          "Unexpected token at " + currentToken.getLinePositionString();
      throw UnexpectedToken(message);
    }
  }
  return StatementNodeUptr{};
}

StatementNodeUptr Parser::parseFunCall(FunctionCallNodeUptr root) {
  std::vector<ExpressionNodeUptr> arguments = parseFunCallArguments();
  root->setArgumentsNodes(arguments);
  expect(Token::TokenType::CloseRoundBracketToken);
  shiftToken();
  return root;
}

std::vector<ExpressionNodeUptr> Parser::parseFunCallArguments() {
  std::vector<ExpressionNodeUptr> arguments;
  ExpressionNodeUptr argument;
  while (argument = parseExpression()) {
    arguments.push_back(std::move(argument));
    if (accept(Token::TokenType::CommaToken)) shiftToken();
  }
  return arguments;
}

ExpressionNodeUptr Parser::parseAssignment(AssignmentNodeUptr root) {
  ExpressionNodeUptr expressionNode = parseExpression();
  root->add(std::move(expressionNode));
  return root;
}

// Sfaktoryzowane
//[fun statement] ::= type, 'function', identifier, '(',arguments,')',':',
// indent, \{statement\},[return statement] ;
// [assign expression] ::= identifier,'=' , expression;
//[matrix] ::= 'matrix',\{'[', non zero integer ,']'\},
// identifier, '=', matrix value;
StatementNodeUptr Parser::parseFunStatOrAssignment() {
  if (accept({Token::TokenType::IntegerToken, Token::TokenType::DoubleToken,
              Token::TokenType::MatrixToken, Token::TokenType::TextToken})) {
    Token typeToken = currentToken;
    shiftToken();
    if (accept(Token::TokenType::FunctionToken)) {
      FunctionStatementNodeUptr functionStat =
          std::make_unique<FunctionStatementNode>(currentToken, typeToken);
      shiftToken();
      return parseFunctionStatement(std::move(functionStat));
    } else if (MatrixSizeNodeUptr matrixSize = parseMatrixSize()) {
      MatrixVariableUptr matrixVariable =
          std::make_unique<MatrixVariable>(typeToken);
      matrixVariable->setMatrixSize(std::move(matrixSize));
      return parseMatrixAssignment(std::move(matrixVariable));
    } else {
      VariableNodeUptr variable = std::make_unique<VariableNode>(typeToken);
      return parseAssignExpression(std::move(variable));
    }
  }
  return StatementNodeUptr{};
}

//[assign expression] ::= identifier, '=' , expression;
ExpressionNodeUptr Parser::parseAssignExpression(ExpressionNodeUptr root) {
  if (expect(Token::TokenType::IdentifierToken)) {
    static_cast<VariableNode*>(root.get())
        ->setIdentifier(currentToken.getString());
    shiftToken();
    expect(Token::TokenType::AssignmentOperatorToken);
    ExpressionValueNodeUptr assignmentNode =
        std::make_unique<AssignmentNode>(currentToken);
    assignmentNode->add(std::move(root));
    shiftToken();
    ExpressionNodeUptr expressionNode = parseExpression();
    assignmentNode->add(std::move(expressionNode));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Assignment expression";
  throw UnexpectedToken(message);
  return ExpressionNodeUptr{};
}

// matrix ::= ’matrix’,{’[’, non zero integer ,’]’}, identifier, ’=’,
// matrixvalue;
ExpressionNodeUptr Parser::parseMatrixAssignment(
    MatrixVariableUptr matrixNode) {
  if (expect(Token::TokenType::IdentifierToken)) {
    matrixNode->setIdentifier(currentToken.getString());
    shiftToken();
    expect(Token::TokenType::AssignmentOperatorToken);
    ExpressionValueNodeUptr assignmentNode =
        std::make_unique<AssignmentNode>(currentToken);
    shiftToken();
    MatrixValueNodeUptr matrixValues = parseMatrixValue();
    if (!matrixValues) {
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Matrix values are missing!";
      throw UnexpectedToken(message);
    }
    assignmentNode->add(std::move(matrixNode));
    assignmentNode->add(std::move(matrixValues));
    return assignmentNode;
  }
  std::string message = "Unexpected token at " +
                        currentToken.getLinePositionString() +
                        ". Expecting: Matrix assignment expression";
  throw UnexpectedToken(message);
  return ExpressionNodeUptr{};
}

// matrix value ::= [,{’[’,{number},’]’},]
MatrixValueNodeUptr Parser::parseMatrixValue() {
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    std::vector<ExpressionNodeUptr> matrixValues;
    shiftToken();
    ExpressionNodeUptr expression;
    while (expression = parseExpression()) {
      matrixValues.push_back(std::move(expression));
      if (!accept(Token::TokenType::CommaToken)) break;
      shiftToken();
    }
    expect(Token::TokenType::CloseSquareBracketToken);
    shiftToken();
    return std::make_unique<MatrixValueNode>(matrixValues);
  }
  return {};
}

//,{’[’, non zero integer ,’]’},
MatrixSizeNodeUptr Parser::parseMatrixSize() {
  std::vector<ExpressionNodeUptr> sizes;
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    shiftToken();
    ExpressionNodeUptr expression;
    expression = parseExpression();
    sizes.push_back(std::move(expression));
    expect(Token::TokenType::CloseSquareBracketToken);
    shiftToken();
    expect(Token::TokenType::OpenSquareBracketToken);
    shiftToken();
    expression = parseExpression();
    sizes.push_back(std::move(expression));
    expect(Token::TokenType::CloseSquareBracketToken);
    shiftToken();
    MatrixSizeNodeUptr matrixSizeNode = std::make_unique<MatrixSizeNode>(sizes);
    return matrixSizeNode;
  }
  return MatrixSizeNodeUptr{};
}

// sum ::= term | sum, {(’-’|’+’) sum}
ExpressionNodeUptr Parser::parseExpression() {
  ExpressionNodeUptr leftSide = parseTerm();
  AdditiveOperatorNodeUptr additiveOperatorNode;
  while (accept(Token::TokenType::AdditiveOperatorToken)) {
    if (!additiveOperatorNode) {
      additiveOperatorNode =
          std::make_unique<AdditiveOperatorNode>(currentToken);
      additiveOperatorNode->add(std::move(leftSide));
    }
    shiftToken();
    ExpressionNodeUptr rightSide = parseExpression();
    additiveOperatorNode->add(std::move(rightSide));
  }
  if (!additiveOperatorNode) return leftSide;
  return additiveOperatorNode;
}

// term ::= factor, {(’*’| ’/’), factor}
ExpressionNodeUptr Parser::parseTerm() {
  ExpressionNodeUptr leftSide = parseFactor();
  MultiplicativeOperatorNodeUptr multiplicativeOperatorNode;
  while (accept(Token::TokenType::MultiplicativeOperatorToken)) {
    if (!multiplicativeOperatorNode) {
      multiplicativeOperatorNode =
          std::make_unique<MultiplicativeOperatorNode>(currentToken);
      multiplicativeOperatorNode->add(std::move(leftSide));
    }
    shiftToken();
    ExpressionNodeUptr rightSide = parseTerm();
    multiplicativeOperatorNode->add(std::move(rightSide));
  }
  if (!multiplicativeOperatorNode) return leftSide;
  return multiplicativeOperatorNode;
}
// factor ::= base { [’ˆ’,exponent] }
ExpressionNodeUptr Parser::parseFactor() {
  ExpressionNodeUptr leftSide = parseParenthesesExpression();
  ExponentiationOperatorNodeUptr exponentiationOperatorNode;
  while (accept(Token::TokenType::ExponentiationOperatorToken)) {
    if (!exponentiationOperatorNode) {
      exponentiationOperatorNode =
          std::make_unique<ExponentiationOperatorNode>(currentToken);
      exponentiationOperatorNode->add(std::move(leftSide));
    }

    shiftToken();
    ExpressionNodeUptr rightSide = parseFactor();
    exponentiationOperatorNode->add(std::move(rightSide));
  }
  if (!exponentiationOperatorNode) return leftSide;
  return exponentiationOperatorNode;
}

// base ::= ’(’ sum ’)’ | identifier | number
ExpressionNodeUptr Parser::parseParenthesesExpression() {
  if (accept(Token::TokenType::IdentifierToken)) {
    ExpressionNodeUptr identifierNode =
        std::make_unique<IdentifierNode>(currentToken);
    shiftToken();
    return identifierNode;
  } else if (accept(Token::TokenType::IntegerLiteralToken)) {
    ExpressionNodeUptr integerNode = std::make_unique<ValueNode>(currentToken);
    shiftToken();
    return integerNode;
  } else if (accept(Token::TokenType::DoubleLiteralToken)) {
    ExpressionNodeUptr doubleNode = std::make_unique<ValueNode>(currentToken);
    shiftToken();
    return doubleNode;
  } else if (accept(Token::TokenType::StringLiteralToken)) {
    ExpressionNodeUptr stringNode = std::make_unique<ValueNode>(currentToken);
    shiftToken();
    return stringNode;
  } else if (accept(Token::TokenType::OpenRoundBracketToken)) {
    ExpressionNodeUptr expressionNode = parseExpression();
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    return expressionNode;
  } else if (accept(Token::TokenType::AdditiveOperatorToken)) {
    AdditiveOperatorNodeUptr additiveOperatorNode =
        std::make_unique<AdditiveOperatorNode>(currentToken);
    shiftToken();
    ExpressionNodeUptr factorNode = parseFactor();
    additiveOperatorNode->add(std::move(factorNode));
    return additiveOperatorNode;
  } else if (accept({Token::TokenType::DetToken, Token::TokenType::TransToken,
                     Token::TokenType::InvToken})) {
    MatrixOperatorNodeUptr matrixOperatorNode =
        std::make_unique<MatrixOperatorNode>(currentToken);
    shiftToken();
    ExpressionNodeUptr expressionNode = parseExpression();
    matrixOperatorNode->add(std::move(expressionNode));
    return matrixOperatorNode;
  } else if (accept(Token::TokenType::OpenSquareBracketToken)) {
    MatrixValueNodeUptr node = parseMatrixValue();
    return node;
  } else if (accept(Token::TokenType::NotToken)) {
    LogicalOperatorNodeUptr logicalNode =
        std::make_unique<LogicalOperatorNode>(currentToken);
    shiftToken();
    ExpressionNodeUptr expressionNode = parseMultipleTestExpressions();
    logicalNode->add(std::move(expressionNode));
    return logicalNode;
  } else {
    return ExpressionNodeUptr{};
  }
}

ExpressionNodeUptr Parser::parseTestExpression() {
  ExpressionNodeUptr leftHandExpression = parseExpression();
  LogicalOperatorNodeUptr testNode;
  while (accept(Token::TokenType::LogicalOperatorToken)) {
    if (!testNode) {
      testNode = std::make_unique<LogicalOperatorNode>(currentToken);
      testNode->add(std::move(leftHandExpression));
    }
    shiftToken();
    ExpressionNodeUptr rightHandExpression = parseTestExpression();
    testNode->add(std::move(rightHandExpression));
  }
  if (!testNode) return leftHandExpression;
  return testNode;
}

ExpressionNodeUptr Parser::parseMultipleTestExpressions() {
  ExpressionNodeUptr leftHandExpression = parseTestExpression();
  LogicalOperatorNodeUptr testNode;
  while (accept({Token::TokenType::AndToken, Token::TokenType::OrToken})) {
    if (!testNode) {
      testNode = std::make_unique<LogicalOperatorNode>(currentToken);
      testNode->add(std::move(leftHandExpression));
    }
    shiftToken();
    ExpressionNodeUptr rightHandExpression = parseMultipleTestExpressions();
    testNode->add(std::move(rightHandExpression));
  }
  if (!testNode) return leftHandExpression;
  return testNode;
}
