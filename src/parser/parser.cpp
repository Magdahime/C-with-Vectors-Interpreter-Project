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
}

StatementNodeUptr Parser::parseStatement() {
  StatementNodeUptr node;
  if ((node = parseLoopStatement()) || (node = parseReturnStatement()) ||
      (node = parseFunStatOrAssignment()) || (node = parseAsLAsStatement())) {
    return node;
  }
  if (!parseEndOfFile()) {
    std::string message =
        "Token at " + currentToken.getLinePositionString() + " is unexpected.";
    throw UnexpectedToken(message);
  }
  return StatementNodeUptr{};
}

bool Parser::parseEndOfFile() {
  return expect(Token::TokenType::EndOfFileToken);
}

// //’if’, parentheses expr , ’:’, indent ,{statement} [other statement];
// NodeUptr Parser::parseIfStatement() {
//   if (accept(Token::TokenType::IfToken)) {
//     NodeUptr ifNode = std::make_unique<IfStatementNode>(currentToken);
//     shiftToken();
//     expect(Token::TokenType::OpenRoundBracketToken);
//     shiftToken();
//     NodeUptr expressionNode = parseMultipleTestExpressions();
//     ifNode->add(std::move(expressionNode));
//     expect(Token::TokenType::CloseRoundBracketToken);
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr ifStatementsNodes;
//     while (ifStatementsNodes = parseStatement()) {
//       ifNode->add(std::move(ifStatementsNodes));
//     }
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     NodeUptr otherwiseNode = parseOtherwiseStatement();
//     if (otherwiseNode) ifNode->add(std::move(otherwiseNode));
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return ifNode;
//   }
//   return NodeUptr{};
// }
// // other statement ::= ’otherwise’, ’:’, indent, {statement};
// NodeUptr Parser::parseOtherwiseStatement() {
//   if (accept(Token::TokenType::OtherwiseToken)) {
//     NodeUptr otherwiseNode = std::make_unique<RootNode>();
//     otherwiseNode->add(std::make_unique<StatementNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr otherwiseStatementsNodes;
//     while (otherwiseStatementsNodes = parseStatement()) {
//       otherwiseNode->add(std::move(otherwiseStatementsNodes));
//     }
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return otherwiseNode;
//   }
//   return NodeUptr{};
// }
// // cond statement ::= ’condition’, ’:’, indent, {case statement},[default
// // statement];
// NodeUptr Parser::parseConditionStatement() {
//   if (accept(Token::TokenType::ConditionToken)) {
//     NodeUptr conditionNode = std::make_unique<RootNode>();
//     conditionNode->add(std::make_unique<StatementNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr caseStatementsNodes;
//     while (caseStatementsNodes = parseCaseStatement()) {
//       conditionNode->add(std::move(caseStatementsNodes));
//     }
//     NodeUptr defaultStatement = parseDefaultStatement();
//     if (defaultStatement) conditionNode->add(std::move(defaultStatement));
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return conditionNode;
//   }
//   return NodeUptr{};
// }
// // case statement ::= ’case’, parentheses expr, ’:’, indent, {statement};
// NodeUptr Parser::parseCaseStatement() {
//   if (accept(Token::TokenType::CaseToken)) {
//     NodeUptr caseNode = std::make_unique<RootNode>();
//     caseNode->add(std::make_unique<StatementNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::OpenRoundBracketToken);
//     shiftToken();
//     NodeUptr expressionNode = parseMultipleTestExpressions();
//     caseNode->add(std::move(expressionNode));
//     expect(Token::TokenType::CloseRoundBracketToken);
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr caseStatementsNodes;
//     while (caseStatementsNodes = parseStatement()) {
//       caseNode->add(std::move(caseStatementsNodes));
//     }
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return caseNode;
//   }
//   return NodeUptr{};
// }

// // default statement ::= ’default’, ’:’, indent, { statement };
// NodeUptr Parser::parseDefaultStatement() {
//   if (accept(Token::TokenType::DefaultToken)) {
//     NodeUptr defaultNode = std::make_unique<RootNode>();
//     defaultNode->add(std::make_unique<StatementNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr defaultStatementsNodes;
//     while (defaultStatementsNodes = parseStatement()) {
//       defaultNode->add(std::move(defaultStatementsNodes));
//     }
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return defaultNode;
//   }
//   return NodeUptr{};
// }

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
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message);
    }
    loopNode->setStart(currentToken.getValue());
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    if (!(accept(Token::TokenType::IntegerLiteralToken) ||
          accept(Token::TokenType::IdentifierToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: identifier or integerLiteral token";
      throw UnexpectedToken(message);
    }
    loopNode->setEnd(currentToken.getValue());
    shiftToken();
    if (accept(Token::TokenType::ColonToken)) {
      shiftToken();
      if (!(accept(Token::TokenType::IntegerLiteralToken) ||
            accept(Token::TokenType::IdentifierToken))) {
        std::string message =
            "Token at " + currentToken.getLinePositionString() +
            " is unexpected. Expecting: identifier or integerLiteral token";
        throw UnexpectedToken(message);
      }
      loopNode->setStep(currentToken.getValue());
      shiftToken();
    }
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    expect(Token::TokenType::ColonToken);
    shiftToken();
    expect(Token::TokenType::OpenBlockToken);
    shiftToken();
    StatementNodeUptr loopStatementsNode;
    while (loopStatementsNode = parseStatement()) {
      loopNode->add(std::move(loopStatementsNode));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return loopNode;
  }
  return StatementNodeUptr{};
}

// aslas statement ::= ’asLongAs’, parentheses expr, ’:’, indent,
//{statement}; 
StatementNodeUptr Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    AslasStatementNode asLasNode = std::make_unique<RootNode>();
    asLasNode->add(std::make_unique<StatementNode>(currentToken));
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
    NodeUptr asLasStatementsNode;
    while (asLasStatementsNode = parseStatement()) {
      asLasNode->add(std::move(asLasStatementsNode));
    }

    expect(
        {Token::TokenType::EndOfFileToken,
        Token::TokenType::CloseBlockToken});
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
    returnStatementNode->add(std::make_unique<ValueNode>(currentToken));
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

// NodeUptr Parser::parseFunCallOrAssignment() {
//   if (accept(Token::TokenType::IdentifierToken)) {
//     NodeUptr rootNode = std::make_unique<RootNode>();
//     rootNode->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     if (accept(Token::TokenType::OpenRoundBracketToken)) {
//       shiftToken();
//       return parseFunCall(std::move(rootNode));
//     } else if (accept(Token::TokenType::AssignmentOperatorToken)) {
//       shiftToken();
//       NodeUptr assignmentNode =
//           std::make_unique<AssigmentOperatorNode>(currentToken);
//       assignmentNode->add(rootNode->getChildren());
//       return parseAssignment(std::move(assignmentNode));
//     } else {
//       std::string message =
//           "Unexpected token at " + currentToken.getLinePositionString();
//       throw UnexpectedToken(message);
//     }
//   }
//   return NodeUptr{};
// }

// NodeUptr Parser::parseFunCall(NodeUptr root) {
//   NodeUptr arguments = parseFunCallArguments();
//   root->add(std::move(arguments));
//   expect(Token::TokenType::CloseRoundBracketToken);
//   shiftToken();
//   return root;
// }

// NodeUptr Parser::parseFunCallArguments() {
//   NodeUptr arguments = std::make_unique<RootNode>();
//   while (accept({Token::TokenType::IntegerLiteralToken,
//                  Token::TokenType::DoubleLiteralToken,
//                  Token::TokenType::StringLiteralToken,
//                  Token::TokenType::IdentifierToken})) {
//     arguments->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     accept(Token::TokenType::CommaToken);
//     shiftToken();
//   }
//   return arguments;
// }

// NodeUptr Parser::parseAssignment(NodeUptr root) {
//   NodeUptr expressionNode = parseExpression();
//   root->add(std::move(expressionNode));
//   return root;
// }

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
        std::make_unique<AssigmentNode>(currentToken);
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
        std::make_unique<AssigmentNode>(currentToken);
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
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      shiftToken();
      ExpressionNodeUptr expression;
      while (expression = parseExpression()) {
        matrixValues.push_back(std::move(expression));
        if (!accept(Token::TokenType::CommaToken)) break;
        shiftToken();
      }
      expect(Token::TokenType::CloseSquareBracketToken);
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
    while (accept(Token::TokenType::OpenSquareBracketToken)) {
      shiftToken();
      expression = parseExpression();
      sizes.push_back(std::move(expression));
      expect(Token::TokenType::CloseSquareBracketToken);
      shiftToken();
    }
    MatrixSizeNodeUptr matrixSizeNode = std::make_unique<MatrixSizeNode>(sizes);
    return matrixSizeNode;
  }
  return MatrixSizeNodeUptr{};
}

// sum ::= term | sum, {(’-’|’+’) sum}
ExpressionNodeUptr Parser::parseExpression() {
  ExpressionNodeUptr leftSide = parseTerm();
  ExpressionValueNodeUptr additiveOperatorNode =
      std::make_unique<AdditiveOperatorNode>(currentToken);
  while (accept(Token::TokenType::AdditiveOperatorToken)) {
    if (additiveOperatorNode->getChildren().size() == 0)
      additiveOperatorNode->add(std::move(leftSide));
    shiftToken();
    ExpressionNodeUptr rightSide = parseExpression();
    additiveOperatorNode->add(std::move(rightSide));
  }
  if (additiveOperatorNode->getChildren().empty()) return leftSide;
  return additiveOperatorNode;
}

// term ::= factor, {(’*’| ’/’), factor}
ExpressionNodeUptr Parser::parseTerm() {
  ExpressionNodeUptr leftSide = parseFactor();
  ExpressionValueNodeUptr multiplicativeOperatorNode =
      std::make_unique<MultiplicativeOperatorNode>(currentToken);
  while (accept(Token::TokenType::MultiplicativeOperatorToken)) {
    if (multiplicativeOperatorNode->getChildren().size() == 0)
      multiplicativeOperatorNode->add(std::move(leftSide));
    shiftToken();
    ExpressionNodeUptr rightSide = parseTerm();
    multiplicativeOperatorNode->add(std::move(rightSide));
  }
  if (multiplicativeOperatorNode->getChildren().empty()) return leftSide;
  return multiplicativeOperatorNode;
}
// factor ::= base { [’ˆ’,exponent] }
ExpressionNodeUptr Parser::parseFactor() {
  ExpressionNodeUptr leftSide = parseParenthesesExpression();
  ExpressionValueNodeUptr exponentiationOperatorNode =
      std::make_unique<ExponentiationOperatorNode>(currentToken);
  while (accept(Token::TokenType::ExponentiationOperatorToken)) {
    if (exponentiationOperatorNode->getChildren().size() == 0)
      exponentiationOperatorNode->add(std::move(leftSide));
    shiftToken();
    ExpressionNodeUptr rightSide = parseFactor();
    exponentiationOperatorNode->add(std::move(rightSide));
  }
  if (exponentiationOperatorNode->getChildren().empty()) return leftSide;
  return exponentiationOperatorNode;
}

// base ::= ’(’ sum ’)’ | identifier | number
ExpressionNodeUptr Parser::parseParenthesesExpression() {
  if (accept(Token::TokenType::IdentifierToken)) {
    ExpressionNodeUptr identifierNode =
        std::make_unique<ValueNode>(currentToken);
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
  } else if (accept(Token::TokenType::OpenRoundBracketToken)) {
    ExpressionNodeUptr expressionNode = parseExpression();
    expect(Token::TokenType::CloseRoundBracketToken);
    shiftToken();
    return expressionNode;
  } else if (accept(Token::TokenType::AdditiveOperatorToken)) {
    ExpressionValueNodeUptr additiveOperatorNode =
        std::make_unique<AdditiveOperatorNode>(currentToken);
    shiftToken();
    ExpressionNodeUptr factorNode = parseFactor();
    additiveOperatorNode->add(std::move(parseFactor()));
    return additiveOperatorNode;
  } else {
    std::string message = "Unexpected token at " +
                          currentToken.getLinePositionString() +
                          ". Expecting: Expression";
    throw UnexpectedToken(message);
  }
  return ExpressionNodeUptr{};
}

ExpressionNodeUptr Parser::parseTestExpression() {
  ExpressionNodeUptr leftHandExpression = parseExpression();
  ExpressionValueNodeUptr testNode =
      std::make_unique<LogicalOperatorNode>(currentToken);
  while (accept(Token::TokenType::LogicalOperatorToken)) {
    if (testNode->getChildren().size() == 0)
      testNode->add(std::move(leftHandExpression));
    shiftToken();
    ExpressionNodeUptr rightHandExpression = parseTestExpression();

    testNode->add(std::move(rightHandExpression));
  }
  if (testNode->getChildren().empty()) return leftHandExpression;
  return testNode;
}

ExpressionNodeUptr Parser::parseMultipleTestExpressions() {
  ExpressionNodeUptr leftHandExpression = parseTestExpression();
  ExpressionValueNodeUptr testNode =
      std::make_unique<LogicalOperatorNode>(currentToken);
  while (accept({Token::TokenType::AndToken, Token::TokenType::OrToken,
                 Token::TokenType::NotToken})) {
    if (testNode->getChildren().size() == 0)
      testNode->add(std::move(leftHandExpression));
    ExpressionNodeUptr rightHandExpression = parseMultipleTestExpressions();
    testNode->add(std::move(rightHandExpression));
  }
  if (testNode->getChildren().empty()) return leftHandExpression;
  return testNode;
}
