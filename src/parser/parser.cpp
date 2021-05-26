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
  NodeUptr node;
  while (node = parseStatement()) {
    static_cast<StatementNode*>(programNode.get())->add(std::move(node));
  }
}

NodeUptr Parser::parseStatement() {
  NodeUptr node;
  if (node = parseLoopStatement()) {
    return node;
  }
  if (!parseEndOfFile()) {
    std::string message =
        "Token at " + currentToken.getLinePositionString() + " is unexpected.";
    throw UnexpectedToken(message);
  }
  return NodeUptr{};
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
NodeUptr Parser::parseLoopStatement() {
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
    NodeUptr loopStatementsNode;
    while (loopStatementsNode = parseStatement()) {
      loopNode->add(std::move(loopStatementsNode));
    }
    expect(
        {Token::TokenType::EndOfFileToken, Token::TokenType::CloseBlockToken});
    shiftToken();
    return loopNode;
  }
  return NodeUptr{};
}

// // aslas statement ::= ’asLongAs’, parentheses expr, ’:’, indent,
// {statement}; NodeUptr Parser::parseAsLAsStatement() {
//   if (accept(Token::TokenType::AsLongAsToken)) {
//     NodeUptr asLasNode = std::make_unique<RootNode>();
//     asLasNode->add(std::make_unique<StatementNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::OpenRoundBracketToken);
//     shiftToken();
//     NodeUptr expressionNode = parseMultipleTestExpressions();
//     asLasNode->add(std::move(expressionNode));
//     expect(Token::TokenType::CloseRoundBracketToken);
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr asLasStatementsNode;
//     while (asLasStatementsNode = parseStatement()) {
//       asLasNode->add(std::move(asLasStatementsNode));
//     }

//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return asLasNode;
//   }
//   return NodeUptr{};
// }
// // fun statement ::= type, ’function’, identifier, ’(’,arguments,’)’,’:’,
// // indent, {statement};
// NodeUptr Parser::parseFunctionStatement(NodeUptr root) {
//   if (accept(Token::TokenType::IdentifierToken)) {
//     root->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::OpenRoundBracketToken);
//     shiftToken();
//     NodeUptr functionArgumentsNode = parseArguments();
//     if (functionArgumentsNode) root->add(std::move(functionArgumentsNode));
//     expect(Token::TokenType::CloseRoundBracketToken);
//     shiftToken();
//     expect(Token::TokenType::ColonToken);
//     shiftToken();
//     expect(Token::TokenType::OpenBlockToken);
//     shiftToken();
//     NodeUptr functionStatementsNode;
//     while (functionStatementsNode = parseStatement()) {
//       root->add(std::move(functionStatementsNode));
//     }
//     NodeUptr functionReturnStatementsNode = parseReturnStatement();
//     if (functionReturnStatementsNode)
//       root->add(functionReturnStatementsNode->getChildren());
//     expect(
//         {Token::TokenType::EndOfFileToken,
//         Token::TokenType::CloseBlockToken});
//     shiftToken();
//     return root;
//   }
//   std::string message = "Token at " + currentToken.getLinePositionString() +
//                         " is unexpected. Expecting : Function statement";
//   throw UnexpectedToken(message);
//   return NodeUptr{};
// }
// //[return statement] ::= 'return', identifier | expression | number | matrix
// |
// // string;
// NodeUptr Parser::parseReturnStatement() {
//   NodeUptr returnStatementNode = std::make_unique<RootNode>();
//   if (accept(Token::TokenType::ReturnToken)) {
//     shiftToken();
//     if (!(accept({Token::TokenType::IntegerLiteralToken,
//                   Token::TokenType::DoubleLiteralToken,
//                   Token::TokenType::StringLiteralToken}) ||
//           accept(Token::TokenType::IdentifierToken))) {
//       NodeUptr node = parseMatrixValue();
//       if (node) return node;
//       std::string message = "Unexpected token at " +
//                             currentToken.getLinePositionString() +
//                             ". Expecting: return value.";
//       throw UnexpectedToken(message);
//     }
//     returnStatementNode->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     return returnStatementNode;
//   }
//   return NodeUptr{};
// }

// // arguments ::= type, identifier [’=’, ’value’] {’,’, type, identifier [’=’,
// // ’value’]};
// NodeUptr Parser::parseArguments() {
//   NodeUptr argumentsNode = std::make_unique<RootNode>();
//   while (accept({Token::TokenType::IntegerToken,
//   Token::TokenType::DoubleToken,
//                  Token::TokenType::MatrixToken,
//                  Token::TokenType::TextToken})) {
//     shiftToken();
//     expect(Token::TokenType::IdentifierToken);
//     argumentsNode->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     NodeUptr defaultArgument = parseDefaultArgument();
//     if (defaultArgument) {
//       argumentsNode->add(defaultArgument->getChildren());
//     }
//     if (!accept(Token::TokenType::CommaToken)) break;
//   }
//   if (!argumentsNode->empty()) return argumentsNode;
//   return NodeUptr{};
// }

// //[arguments] ::= type, identifier ['=', 'value'] \{',', type, identifier
// ['=',
// //'value']\};
// NodeUptr Parser::parseDefaultArgument() {
//   if (accept(Token::TokenType::AssignmentOperatorToken)) {
//     shiftToken();
//     NodeUptr argumentsNode = std::make_unique<RootNode>();
//     if (!accept({Token::TokenType::IntegerLiteralToken,
//                  Token::TokenType::DoubleLiteralToken,
//                  Token::TokenType::StringLiteralToken})) {
//       NodeUptr node = parseMatrixValue();
//       if (node)
//         argumentsNode->add(std::move(node));
//       else {
//         std::string message = "Unexpected token at " +
//                               currentToken.getLinePositionString() +
//                               ". Expecting: literal or identifier.";
//         throw UnexpectedToken(message);
//       }

//     } else {
//       argumentsNode->add(std::make_unique<LiteralNode>(currentToken));
//       shiftToken();
//     }
//     return argumentsNode;
//   }
//   return NodeUptr{};
// }

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

// // Sfaktoryzowane
// //[fun statement] ::= type, 'function', identifier, '(',arguments,')',':',
// // indent, \{statement\},[return statement] ; [assign expression] ::=
// // identifier,
// //'=' , expression; [matrix] ::= 'matrix',\{'[', non zero integer ,']'\},
// // identifier, '=', matrix value;
// NodeUptr Parser::parseFunStatOrAssignment() {
//   NodeUptr funOrAs = std::make_unique<RootNode>();
//   if (accept({Token::TokenType::IntegerToken, Token::TokenType::DoubleToken,
//               Token::TokenType::MatrixToken, Token::TokenType::TextToken})) {
//     funOrAs->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     if (accept(Token::TokenType::FunctionToken)) {
//       funOrAs->add(std::make_unique<StatementNode>(currentToken));
//       shiftToken();
//       return parseFunctionStatement(std::move(funOrAs));
//     } else if (NodeUptr matrixSize = parseMatrixSize()) {
//       funOrAs->add(std::move(matrixSize));
//       shiftToken();
//       return parseMatrixAssignment(std::move(funOrAs));
//     } else {
//       return parseAssignExpression(std::move(funOrAs));
//     }
//   }
//   return NodeUptr{};
// }

// //[assign expression] ::= identifier, '=' , expression;
// NodeUptr Parser::parseAssignExpression(NodeUptr root) {
//   if (expect(Token::TokenType::IdentifierToken)) {
//     root->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::AssignmentOperatorToken);
//     shiftToken();
//     NodeUptr assignmentNode =
//         std::make_unique<AssigmentOperatorNode>(currentToken);
//     assignmentNode->add(root->getChildren());
//     NodeUptr expressionNode = parseExpression();
//     assignmentNode->add(std::move(expressionNode));
//     return assignmentNode;
//   }
//   std::string message = "Unexpected token at " +
//                         currentToken.getLinePositionString() +
//                         ". Expecting: Assignment expression";
//   throw UnexpectedToken(message);
//   return NodeUptr{};
// }

// // matrix ::= ’matrix’,{’[’, non zero integer ,’]’}, identifier, ’=’,
// // matrixvalue;
// NodeUptr Parser::parseMatrixAssignment(NodeUptr matrixNode) {
//   if (expect(Token::TokenType::IdentifierToken)) {
//     matrixNode->add(std::make_unique<LiteralNode>(currentToken));
//     shiftToken();
//     expect(Token::TokenType::AssignmentOperatorToken);
//     shiftToken();
//     NodeUptr assignmentNode =
//         std::make_unique<AssigmentOperatorNode>(currentToken);
//     assignmentNode->add(matrixNode->getChildren());
//     NodeUptr matrixValues;
//     matrixValues = parseMatrixValue();
//     if (matrixValues->empty()) {
//       std::string message = "Unexpected token at " +
//                             currentToken.getLinePositionString() +
//                             ". Matrix values are missing!";
//       throw UnexpectedToken(message);
//     }
//     assignmentNode->add(std::move(matrixValues));
//     return assignmentNode;
//   }
//   std::string message = "Unexpected token at " +
//                         currentToken.getLinePositionString() +
//                         ". Expecting: Matrix assignment expression";
//   throw UnexpectedToken(message);
//   return NodeUptr{};
// }

// // matrix value ::= [,{’[’,{number},’]’},]
// NodeUptr Parser::parseMatrixValue() {
//   NodeUptr matrixValues = std::make_unique<RootNode>();
//   if (accept(Token::TokenType::OpenSquareBracketToken)) {
//     shiftToken();
//     while (accept(Token::TokenType::OpenSquareBracketToken)) {
//       shiftToken();
//       while (accept(Token::TokenType::IntegerLiteralToken) ||
//              accept(Token::TokenType::DoubleToken)) {
//         matrixValues->add(std::make_unique<LiteralNode>(currentToken));
//         shiftToken();
//         accept(Token::TokenType::CommaToken);
//         shiftToken();
//       }
//       expect(Token::TokenType::CloseSquareBracketToken);
//       shiftToken();
//     }
//     expect(Token::TokenType::CloseSquareBracketToken);
//     shiftToken();
//     return matrixValues;
//   }
//   return NodeUptr{};
// }
// //,{’[’, non zero integer ,’]’},
// NodeUptr Parser::parseMatrixSize() {
//   NodeUptr matrixSizeNode = std::make_unique<RootNode>();
//   if (accept(Token::TokenType::OpenSquareBracketToken)) {
//     shiftToken();
//     expect(Token::TokenType::IntegerLiteralToken);
//     shiftToken();
//     matrixSizeNode->add(std::make_unique<LiteralNode>(currentToken));
//     expect(Token::TokenType::CloseSquareBracketToken);
//     shiftToken();
//     while (accept(Token::TokenType::OpenSquareBracketToken)) {
//       shiftToken();
//       expect(Token::TokenType::IntegerLiteralToken);
//       shiftToken();
//       matrixSizeNode->add(std::make_unique<LiteralNode>(currentToken));
//       expect(Token::TokenType::CloseSquareBracketToken);
//       shiftToken();
//     }
//     return matrixSizeNode;
//   }
//   return NodeUptr{};
// }

// // sum ::= term | sum, {(’-’|’+’) sum}
// NodeUptr Parser::parseExpression() {
//   NodeUptr leftSide = parseTerm();
//   NodeUptr additiveOperatorNode;
//   while (accept(Token::TokenType::AdditiveOperatorToken)) {
//     additiveOperatorNode =
//     std::make_unique<AdditiveOperatorNode>(currentToken); shiftToken();
//     NodeUptr rightSide = parseTerm();
//     additiveOperatorNode->add(std::move(leftSide));
//     additiveOperatorNode->add(std::move(rightSide));
//   }
//   if (!additiveOperatorNode) return leftSide;
//   return additiveOperatorNode;
// }

// // term ::= factor, {(’*’| ’/’), factor}
// NodeUptr Parser::parseTerm() {
//   NodeUptr leftSide = parseFactor();
//   NodeUptr multiplicativeOperatorNode;
//   while (accept(Token::TokenType::MultiplicativeOperatorToken)) {
//     multiplicativeOperatorNode =
//         std::make_unique<MultiplicativeOperatorNode>(currentToken);
//     shiftToken();
//     NodeUptr rightSide = parseFactor();
//     multiplicativeOperatorNode->add(std::move(leftSide));
//     multiplicativeOperatorNode->add(std::move(rightSide));
//   }
//   if (!multiplicativeOperatorNode) return leftSide;
//   return multiplicativeOperatorNode;
// }
// // factor ::= base { [’ˆ’,exponent] }
// NodeUptr Parser::parseFactor() {
//   NodeUptr leftSide = parseParenthesesExpression();
//   NodeUptr exponentiationOperatorNode;
//   while (accept(Token::TokenType::ExponentiationOperatorToken)) {
//     exponentiationOperatorNode =
//         std::make_unique<ExponentiationOperatorNode>(currentToken);
//     shiftToken();
//     NodeUptr rightSide = parseFactor();
//     exponentiationOperatorNode->add(std::move(leftSide));
//     exponentiationOperatorNode->add(std::move(rightSide));
//   }
//   if (!exponentiationOperatorNode) return leftSide;
//   return exponentiationOperatorNode;
// }

// // base ::= ’(’ sum ’)’ | identifier | number
// NodeUptr Parser::parseParenthesesExpression() {
//   if (accept(Token::TokenType::IdentifierToken)) {
//     NodeUptr identifierNode = std::make_unique<LiteralNode>(currentToken);
//     shiftToken();
//     return identifierNode;
//   } else if (accept(Token::TokenType::IntegerLiteralToken)) {
//     NodeUptr integerNode = std::make_unique<LiteralNode>(currentToken);
//     shiftToken();
//     return integerNode;
//   } else if (accept(Token::TokenType::DoubleLiteralToken)) {
//     NodeUptr doubleNode = std::make_unique<LiteralNode>(currentToken);
//     shiftToken();
//     return doubleNode;
//   } else if (accept(Token::TokenType::OpenRoundBracketToken)) {
//     NodeUptr expressionNode = parseExpression();
//     expect(Token::TokenType::CloseRoundBracketToken);
//     shiftToken();
//     return expressionNode;
//   } else if (accept(Token::TokenType::AdditiveOperatorToken)) {
//     NodeUptr additiveOperatorNode =
//         std::make_unique<AdditiveOperatorNode>(currentToken);
//     shiftToken();
//     NodeUptr factorNode = parseFactor();
//     additiveOperatorNode->add(std::move(parseFactor()));
//     return additiveOperatorNode;
//   } else {
//     std::string message = "Unexpected token at " +
//                           currentToken.getLinePositionString() +
//                           ". Expecting: Expression";
//     throw UnexpectedToken(message);
//   }
//   return NodeUptr{};
// }

// NodeUptr Parser::parseTestExpression() {
//   NodeUptr leftHandExpression = parseExpression();
//   NodeUptr testNode;
//   while (accept(Token::TokenType::LogicalOperatorToken)) {
//     testNode = std::make_unique<LogicalOperatorNode>(currentToken);
//     shiftToken();
//     NodeUptr rightHandExpression = parseExpression();
//     testNode->add(std::move(leftHandExpression));
//     testNode->add(std::move(rightHandExpression));
//   }
//   if (!testNode) return leftHandExpression;
//   return testNode;
// }

// NodeUptr Parser::parseMultipleTestExpressions() {
//   NodeUptr leftHandExpression = parseTestExpression();
//   NodeUptr testNode;
//   while (accept({Token::TokenType::AndToken, Token::TokenType::OrToken,
//                  Token::TokenType::NotToken})) {
//     testNode = std::make_unique<LogicalOperatorNode>(currentToken);
//     NodeUptr rightHandExpression = parseTestExpression();
//     testNode->add(std::move(leftHandExpression));
//     testNode->add(std::move(rightHandExpression));
//   }
//   if (!testNode) return leftHandExpression;
//   return testNode;
// }

// std::string Parser::getProgramString(){
// }