#include "parser/parser.hpp"

bool Parser::accept(const Token::TokenType& token) {
  if (currentToken == token) {
    currentToken = *lexer.getToken();
    return true;
  }
  return false;
}
bool Parser::accept(const std::vector<Token::TokenType> acceptedTokens) {
  for (auto& token : acceptedTokens) {
    if (currentToken == token) return true;
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

std::optional<NodeUptr> Parser::parseProgram() {
  std::optional<NodeUptr> node;
  while (true) {
    node = *parseStatement();
    if (node)
      programNode->add(*node);
    else
      break;
  }
  expect(Token::TokenType::EndOfFileToken);
  return {};
}

std::optional<NodeUptr> Parser::parseStatement() { return {}; }

std::optional<NodeUptr> Parser::parseIfStatement() {
  if (accept(Token::TokenType::IfToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    if (accept(Token::TokenType::OtherwiseToken)) parseOtherwiseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseOtherwiseStatement() {
  if (accept(Token::TokenType::OtherwiseToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseConditionStatement() {
  if (accept(Token::TokenType::ConditionToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseCaseStatement();
    parseDefaultStatement();
    expect(Token::TokenType::CloseBlockToken);
  }

  return {};
}

std::optional<NodeUptr> Parser::parseCaseStatement() {
  if (accept(Token::TokenType::CaseToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}
std::optional<NodeUptr> Parser::parseDefaultStatement() {
  if (accept(Token::TokenType::DefaultToken)) {
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseLoopStatement() {
  if (accept(Token::TokenType::LoopToken)) {
    expect(Token::TokenType::OpenRoundBracketToken);
    expect(Token::TokenType::IntegerLiteralToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::IntegerLiteralToken);
    parseLoopStepStatement();
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseLoopStepStatement() {
  if (accept(Token::TokenType::ColonToken)) {
    // TODO SPRAWDZENIE CZY WIĘKSZE OD 0
    // TO NIEŁADNE
    if (std::get<int64_t>(currentToken.getValue()) <= 0) {
      std::string message = "Integer literal at " +
                            currentToken.getLinePositionString() +
                            " should be above 0!";
      throw UnexpectedToken(message.c_str());
    }
    expect(Token::TokenType::IntegerLiteralToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseAsLAsStatement() {
  if (accept(Token::TokenType::AsLongAsToken)) {
    parseParenthesesExpression();
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseFunctionStatement() {
  Token returnToken = currentToken;
  if (accept(typeTokens)) {
    expect(Token::TokenType::FunctionToken);
    expect(Token::TokenType::IdentifierToken);
    expect(Token::TokenType::OpenRoundBracketToken);
    parseArguments();
    expect(Token::TokenType::CloseRoundBracketToken);
    expect(Token::TokenType::ColonToken);
    expect(Token::TokenType::OpenBlockToken);
    parseStatement();
    parseReturnStatement();
    expect(Token::TokenType::CloseBlockToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseReturnStatement() {
  // TODO SPRAWDZANIE TUTAJ CZY DOBRY TYP ZWARACANY
  // TODO jeśli funckja z voidem to jak coś zwraca to błąd, jak miała coś wracać
  // a nie zwraca to też błąd
  if (accept(Token::TokenType::ReturnToken)) {
    if (!accept(literalsTokens)) {
      std::optional<NodeUptr> node = parseMatrix();
      if (node) return node;
      std::string message = "Unexpected token at " +
                            currentToken.getLinePositionString() +
                            ". Expecting: return value.";
      throw UnexpectedToken(message.c_str());
    }
  }
  return {};
}
// TODO DOKOŃCZYĆ
std::optional<NodeUptr> Parser::parseArguments() { return {}; }
std::optional<NodeUptr> Parser::parseDefaultArgument() {
  if (accept(Token::TokenType::AssignmentOperatorToken)) {
  }
  return {};
}

// TODO DO DOKOŃCZENIA
std::optional<NodeUptr> Parser::parseFunCallOrAssignment() {
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
  return {};
}
// TODO TUTAJ LUB
std::optional<NodeUptr> Parser::parseParenthesesExpression() {
  if (accept(Token::TokenType::OpenRoundBracketToken)) {
    parseFunCallOrAssignment();  // LUB TO DRUGIE
    parseTestExpression();
    expect(Token::TokenType::CloseRoundBracketToken);
  }
  return {};
}

// TODO DOKOŃCZYĆ
std::optional<NodeUptr> Parser::parseTestExpression() { return {}; }

std::optional<NodeUptr> Parser::parseMatrix() {
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
  return {};
}
// TO DO SPRAWDZANIE ILE PODAŁ UŻYTKOWNIK LICZB
std::optional<NodeUptr> Parser::parseMatrixValue() {
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    while (accept(Token::TokenType::IntegerLiteralToken) ||
           accept(Token::TokenType::DoubleToken)) {
    }
    expect(Token::TokenType::CloseSquareBracketToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseMatrixSize() {
  if (accept(Token::TokenType::OpenSquareBracketToken)) {
    expect(Token::TokenType::IntegerLiteralToken);
    expect(Token::TokenType::CloseSquareBracketToken);
  }
  return {};
}

std::optional<NodeUptr> Parser::parseComment() {
  accept(Token::TokenType::CommentToken);
  return {};
}