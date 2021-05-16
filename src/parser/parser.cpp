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
  node = parseFunctionStatement();
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

NodeUptr Parser::parseIfStatement() {
  if (accept(Token::TokenType::IfToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    if (accept(Token::TokenType::OtherwiseToken)) parseOtherwiseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return NodeUptr{};
}

NodeUptr Parser::parseOtherwiseStatement() {
  if (accept(Token::TokenType::OtherwiseToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return NodeUptr{};
}

NodeUptr Parser::parseConditionStatement() {
  if (accept(Token::TokenType::ConditionToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseCaseStatement();
    parseDefaultStatement();
    expect(Token::TokenType::CloseBlockToken);
  }

  return NodeUptr{};
}

NodeUptr Parser::parseCaseStatement() {
  if (accept(Token::TokenType::CaseToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return NodeUptr{};
}
NodeUptr Parser::parseDefaultStatement() {
  if (accept(Token::TokenType::DefaultToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
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
    if (!(accept(Token::TokenType::CloseBlockToken) ||
          accept(Token::TokenType::EndOfFileToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: CloseBlockToken or EndOfFileToken";
      throw UnexpectedToken(message.c_str());
    }
    loopNode->add(std::make_unique<LeafNode>(lastToken));
    return loopNode;
  }
  return NodeUptr{};
}

NodeUptr Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return NodeUptr{};
}
// fun statement ::= type, ’function’, identifier, ’(’,arguments,’)’,’:’,
// indent, {statement};
NodeUptr Parser::parseFunctionStatement() {
  NodeUptr functionNode = std::make_unique<RootNode>();
  if (accept(typeTokens)) {
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    functionNode->printChildren();
    expect(Token::TokenType::FunctionToken);
    functionNode->add(std::make_unique<StatementNode>(lastToken));
    expect(Token::TokenType::IdentifierToken);
    functionNode->add(std::make_unique<LiteralNode>(lastToken));
    expect(Token::TokenType::OpenRoundBracketToken);
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr functionArgumentsNode = parseArguments();
    if (functionArgumentsNode)
      functionNode->add(std::move(functionArgumentsNode));
    expect(Token::TokenType::CloseRoundBracketToken);
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::ColonToken);
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::NextLineToken);
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    expect(Token::TokenType::OpenBlockToken);
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    NodeUptr functionStatementsNode;
    while (functionStatementsNode = parseStatement()) {
      functionNode->add(std::move(functionStatementsNode));
    }
    NodeUptr functionReturnStatementsNode = parseReturnStatement();
    if (functionReturnStatementsNode)
      functionNode->add(functionReturnStatementsNode->getChildren());
    if (!(accept(Token::TokenType::CloseBlockToken) ||
          accept(Token::TokenType::EndOfFileToken))) {
      std::string message =
          "Token at " + currentToken.getLinePositionString() +
          " is unexpected. Expecting: CloseBlockToken or EndOfFileToken";
      throw UnexpectedToken(message.c_str());
    }
    functionNode->add(std::make_unique<LeafNode>(lastToken));
    return functionNode;
  }
  return NodeUptr{};
}

NodeUptr Parser::parseReturnStatement() {
  NodeUptr returnStatementNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::ReturnToken)) {
    returnStatementNode->add(std::make_unique<LeafNode>(lastToken));
    if (!(accept(literalsTokens) ||
          accept(Token::TokenType::IdentifierToken))) {
      NodeUptr node = parseMatrix();
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
NodeUptr Parser::parseDefaultArgument() {
  NodeUptr argumentsNode = std::make_unique<RootNode>();
  if (accept(Token::TokenType::AssignmentOperatorToken)) {
    argumentsNode->add(std::make_unique<LeafNode>(lastToken));
    if (!accept(literalsTokens)) {
      NodeUptr node = parseMatrix();
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

// TODO DO DOKOŃCZENIA
NodeUptr Parser::parseFunCallOrAssignment() {
  if (accept(Token::TokenType::IdentifierToken)) {
    if (accept(Token::TokenType::OpenRoundBracketToken)) {
    } else if (accept(Token::TokenType::AssignmentOperatorToken)) {
    } else {
      char message[100];
      sprintf(message, "Unexpected token at %s",
              currentToken.getLinePositionString().c_str());
      throw UnexpectedToken(message);
    }
  }
  return NodeUptr{};
}
// TODO TUTAJ LUB
NodeUptr Parser::parseParenthesesExpression() {
  if (accept(Token::TokenType::OpenRoundBracketToken)) {
    parseFunCallOrAssignment();  // LUB TO DRUGIE
    parseTestExpression();
    expect(Token::TokenType::CloseRoundBracketToken);
  }
  return NodeUptr{};
}

// TODO DOKOŃCZYĆ
NodeUptr Parser::parseTestExpression() { return NodeUptr{}; }

NodeUptr Parser::parseMatrix() {
  if (accept(Token::TokenType::MatrixToken)) {
    while (parseMatrixSize()) {
    }
    expect(Token::TokenType::IdentifierToken);
    expect(Token::TokenType::AssignmentOperatorToken);
    expect(Token::TokenType::OpenSquareBracketToken);
    while (parseMatrixValue()) {
    }
    expect(Token::TokenType::CloseSquareBracketToken);
  }
  return NodeUptr{};
}
// TO DO SPRAWDZANIE ILE PODAŁ UŻYTKOWNIK LICZB
NodeUptr Parser::parseMatrixValue() {
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    while (accept(Token::TokenType::IntegerLiteralToken) ||
           accept(Token::TokenType::DoubleToken)) {
    }
    expect(Token::TokenType::CloseSquareBracketToken);
  }
  return NodeUptr{};
}

NodeUptr Parser::parseMatrixSize() {
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    expect(Token::TokenType::IntegerLiteralToken);
    expect(Token::TokenType::CloseSquareBracketToken);
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

NodeUptr Parser::parseNewline() {
  if (accept(Token::TokenType::NextLineToken)) {
    return std::make_unique<CommentNode>(lastToken);
  }
  return NodeUptr{};
}