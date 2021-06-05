#include "lexicalAnalyzer/lexicalAnalyzer.hpp"
using namespace Operators;

std::optional<Token> LexicalAnalyzer::getToken() {
  std::optional<Token> nextToken = buildEOF();
  if (nextToken) return nextToken;
  if (!isNextLine) {
    skipWhites();
  } else {
    nextToken = buildIndent();
    this->isNextLine = false;
    if (nextToken) return nextToken;
  }
  nextToken = buildStringLiteral();
  if (nextToken) return nextToken;

  nextToken = buildNumber();
  if (nextToken) return nextToken;

  nextToken = buildIdentifierOrKeyword();
  if (nextToken) return nextToken;

  nextToken = buildComment();
  if (nextToken) return nextToken;

  nextToken = buildDivisionTokenOrComment();
  if (nextToken) return nextToken;

  nextToken = buildOneCharToken();
  if (nextToken) return nextToken;

  nextToken = buildLogicalOperatorToken();
  if (nextToken) return nextToken;

  return buildUnindentified();
}

std::optional<Token> LexicalAnalyzer::buildIdentifierOrKeyword() {
  NextCharacter current = source->getCurrentCharacter();
  if (isalpha(current.nextLetter)) {
    std::stringstream ss;
    ss << current.nextLetter;
    NextCharacter nextCharacter = source->getChar();
    while (isalnum(nextCharacter.nextLetter) ||
           nextCharacter.nextLetter == '_') {
      ss << nextCharacter.nextLetter;
      nextCharacter = source->getChar();
    }
    auto type = LexicalTable::keywordTable.find(ss.str());
    if (type != LexicalTable::keywordTable.end())
      return Token(type->second, TokenVariant(ss.str()), current);
    else
      return Token(Token::TokenType::IdentifierToken, TokenVariant(ss.str()),
                   current);
  }
  return {};
}

std::optional<Token> LexicalAnalyzer::buildEOF() {
  NextCharacter current = source->getCurrentCharacter();
  if (current.nextLetter == '\0') {
    return Token(Token::TokenType::EndOfFileToken, std::monostate{}, current);
  }
  return {};
}

std::optional<Token> LexicalAnalyzer::buildComment() {
  NextCharacter current = source->getCurrentCharacter();
  if (current.nextLetter == '#') {
    std::stringstream ss;
    ss << current.nextLetter;
    uint32_t length = 1;
    NextCharacter nextCharacter = source->getChar();
    while (length <= MAXSIZE && nextCharacter.nextLetter != '\n' &&
           nextCharacter.nextLetter != '\0') {
      ss << nextCharacter.nextLetter;
      nextCharacter = source->getChar();
      ++length;
    }
    if (length >= MAXSIZE) {
      std::string message =
          "Commentary at " + current.getLinePosition() + " is too long.";
      throw TooLongStringLiteral(message);
    }
    return Token(Token::TokenType::CommentToken, TokenVariant(ss.str()),
                 current);
  }
  return {};
}

std::optional<Token> LexicalAnalyzer::buildUnindentified() {
  NextCharacter current = source->getCurrentCharacter();
  return Token(Token::TokenType::UnindentifiedToken, std::monostate{}, current);
}

std::optional<Token> LexicalAnalyzer::buildDivisionTokenOrComment() {
  NextCharacter current = source->getCurrentCharacter();
  if (current.nextLetter == '/') {
    std::stringstream ss;
    ss << current.nextLetter;
    NextCharacter nextCharacter = source->getChar();
    if (nextCharacter.nextLetter == '/') {
      ss << nextCharacter.nextLetter;
      nextCharacter = source->getChar();
      uint32_t length = 2;
      while (length < MAXSIZE && nextCharacter.nextLetter != '\n' &&
             nextCharacter.nextLetter != '\0') {
        ss << nextCharacter.nextLetter;
        nextCharacter = source->getChar();
        ++length;
      }
      if (length >= MAXSIZE) {
        std::string message =
            "Commentary at " + current.getLinePosition() + " is too long.";
        throw TooLongStringLiteral(message);
      }

      return Token(Token::TokenType::CommentToken, TokenVariant(ss.str()),
                   current);
    } else {
      return Token(Token::TokenType::MultiplicativeOperatorToken,
                   Token::TokenSubtype::DivisionToken, std::monostate{},
                   current);
    }
  }
  return {};
}

std::optional<Token> LexicalAnalyzer::buildOneCharToken() {
  NextCharacter current = source->getCurrentCharacter();
  std::optional<Token::TokenType> type;
  std::optional<Token::TokenSubtype> subtype;
  switch (current.nextLetter) {
    case ('+'):
      type = Token::TokenType::AdditiveOperatorToken;
      subtype = Token::TokenSubtype::PlusToken;
      break;
    case ('\n'):
      isNextLine = true;
      type = Token::TokenType::NextLineToken;
      break;
    case ('-'):
      type = Token::TokenType::AdditiveOperatorToken;
      subtype = Token::TokenSubtype::MinusToken;
      break;
    case ('*'):
      type = Token::TokenType::MultiplicativeOperatorToken;
      subtype = Token::TokenSubtype::MultiplicationToken;
      break;
    case ('('):
      type = Token::TokenType::OpenRoundBracketToken;
      break;

    case (')'):
      type = Token::TokenType::CloseRoundBracketToken;
      break;

    case ('['):
      type = Token::TokenType::OpenSquareBracketToken;
      break;

    case (']'):
      type = Token::TokenType::CloseSquareBracketToken;
      break;

    case (':'):
      type = Token::TokenType::ColonToken;
      break;

    case ('.'):
      type = Token::TokenType::PointToken;
      break;

    case (','):
      type = Token::TokenType::CommaToken;
      break;
    case ('^'):
      type = Token::TokenType::ExponentiationOperatorToken;
      break;
  }

  if (subtype) {
    source->getChar();
    return Token(*type, *subtype, std::monostate{}, current);
  } else if (type) {
    source->getChar();
    return Token(*type, std::monostate{}, current);
  }
  return {};
}

std::string LexicalAnalyzer::checkForEscapedSequence(
    NextCharacter &current) const {
  NextCharacter nextCharacter = source->getChar();
  std::string seq = std::string(1, current.nextLetter);
  seq += nextCharacter.nextLetter;
  auto it = LexicalTable::escapeTable.find(seq);
  if (it != LexicalTable::escapeTable.end()) {
    return std::string(1, it->second);
  }
  return seq;
}

std::optional<Token> LexicalAnalyzer::buildStringLiteral() {
  NextCharacter current = source->getCurrentCharacter();
  if (current.nextLetter == '\'' || current.nextLetter == '\"') {
    char delimiter = current.nextLetter;
    std::stringstream ss;
    NextCharacter nextCharacter = source->getChar();
    uint32_t length = 1;
    while (length < MAXSIZE && isprint(nextCharacter.nextLetter) &&
           nextCharacter.nextLetter != '\n' &&
           nextCharacter.nextLetter != delimiter &&
           nextCharacter.nextLetter != '\0') {
      if (nextCharacter.nextLetter == '\\') {
        auto result = checkForEscapedSequence(nextCharacter);
        length += result.length() - 1;
        ss << result;
      } else
        ss << nextCharacter.nextLetter;
      nextCharacter = source->getChar();
      length++;
    }
    if (length >= MAXSIZE) {
      std::string message =
          "String literal at " + current.getLinePosition() + " is too long.";
      throw TooLongStringLiteral(message);
    } else if (nextCharacter.nextLetter == delimiter) {
      source->getChar();
      return Token(Token::TokenType::StringLiteralToken, TokenVariant(ss.str()),
                   current);
    } else {
      std::string message =
          "String literal at " + current.getLinePosition() + " is malformed.";
      throw WronglyDefinedStringLiteral(message);
    }
  }
  return {};
}

std::optional<Token> LexicalAnalyzer::buildNumber() {
  NextCharacter current = source->getCurrentCharacter();
  if (isdigit(current.nextLetter)) {
    int64_t integerToBe = buildInteger(current);
    NextCharacter nextCharacter = source->getCurrentCharacter();
    if (nextCharacter.nextLetter == '.') {
      return Token(Token::TokenType::DoubleLiteralToken,
                   TokenVariant(buildFloatingPointNumber(current, integerToBe)),
                   current);
    } else
      return Token(Token::TokenType::IntegerLiteralToken,
                   TokenVariant(integerToBe), current);
  }
  return {};
}

bool LexicalAnalyzer::checkIfFits(const std::string_view numberToCheck,
                                  const std::string_view limit) const {
  if (numberToCheck.length() > limit.length()) return false;
  if (numberToCheck.length() == limit.length()) {
    for (long unsigned int i = 0; i < limit.length(); i++) {
      if (numberToCheck[i] > limit[i]) return false;
    }
  }

  return true;
}

int64_t LexicalAnalyzer::buildInteger(NextCharacter &current) {
  int64_t integerToBe = current.nextLetter - '0';
  uint64_t base = 10;
  NextCharacter nextCharacter = source->getChar();
  while (isdigit(nextCharacter.nextLetter)) {
    if (!checkIfFits(std::to_string(integerToBe) + nextCharacter.nextLetter,
                     std::to_string(INT64_MAX))) {
      std::string message =
          "Integer constant " + current.getLinePosition() + " is too big.";
      throw IntegerTooBig(message);
    }
    integerToBe *= base;
    integerToBe += nextCharacter.nextLetter - '0';
    nextCharacter = source->getChar();
  }
  return integerToBe;
}
double LexicalAnalyzer::buildFloatingPointNumber(NextCharacter &current,
                                                 int64_t integerPart) {
  if (!checkIfFits(std::to_string(integerPart),
                   std::to_string(std::numeric_limits<double>::max()))) {
    std::string message =
        "Double constant at " + current.getLinePosition() + " is too big.";
    throw IntegerTooBig(message);
  }
  double finalFractionalpart = 0;
  short base = 10;
  NextCharacter nextCharacter = source->getChar();
  if (isdigit(nextCharacter.nextLetter)) {
    int64_t fractionalPart = 0;
    fractionalPart = nextCharacter.nextLetter - '0';
    uint32_t length = 1;
    nextCharacter = source->getChar();
    while (isdigit(nextCharacter.nextLetter)) {
      fractionalPart *= base;
      fractionalPart += nextCharacter.nextLetter - '0';
      ++length;
      nextCharacter = source->getChar();
    }
    finalFractionalpart = fractionalPart / pow(base, length);
  }
  return integerPart + finalFractionalpart;
}

std::optional<Token> LexicalAnalyzer::buildLogicalOperatorToken() {
  NextCharacter current = source->getCurrentCharacter();
  switch (current.nextLetter) {
    case ('<'): {
      NextCharacter nextCharacter = source->getChar();
      if (nextCharacter.nextLetter == '=') {
        source->getChar();
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::LessOrEqualToken, std::monostate{},
                     current);
      } else {
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::LessToken, std::monostate{}, current);
      }
      break;
    }

    case ('>'): {
      NextCharacter nextCharacter = source->getChar();
      if (nextCharacter.nextLetter == '=') {
        source->getChar();
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::GreaterOrEqualToken, std::monostate{},
                     current);
      } else {
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::GreaterToken, std::monostate{},
                     current);
      }
      break;
    }

    case ('='): {
      NextCharacter nextCharacter = source->getChar();
      if (nextCharacter.nextLetter == '=') {
        source->getChar();
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::EqualToken, std::monostate{},
                     current);
      } else {
        return Token(Token::TokenType::AssignmentOperatorToken,
                     std::monostate{}, current);
      }
      break;
    }

    case ('!'): {
      NextCharacter nextCharacter = source->getChar();
      if (nextCharacter.nextLetter == '=') {
        source->getChar();
        return Token(Token::TokenType::LogicalOperatorToken,
                     Token::TokenSubtype::NotEqualToken, std::monostate{},
                     current);
      } else {
        return Token(Token::TokenType::NotToken, std::monostate{}, current);
      }
      break;
    }
  }
  return {};
}

void LexicalAnalyzer::skipWhites() {
  NextCharacter current = source->getCurrentCharacter();
  if (isspace(current.nextLetter) && current.nextLetter != '\n') {
    NextCharacter nextCharacter = source->getChar();
    while (isspace(nextCharacter.nextLetter) && current.nextLetter != '\n') {
      nextCharacter = source->getChar();
    }
  }
}

std::optional<Token> LexicalAnalyzer::buildIndent() {
  NextCharacter current = source->getCurrentCharacter();
  if (current.nextLetter == ' ' || current.nextLetter == '\t') {
    if (chosenIndentChar == 0) {
      chosenIndentChar = current.nextLetter;
    } else if (current.nextLetter != chosenIndentChar) {
      std::string messsage =
          "Inconsistent use of tabs and spaces in indentation at " +
          current.getLinePosition();
      throw NotConsistentIndent(messsage.c_str());
    }
    std::stringstream ss;
    ss << current.nextLetter;
    NextCharacter nextCharacter = source->getChar();
    while (nextCharacter.nextLetter == chosenIndentChar) {
      ss << nextCharacter.nextLetter;
      nextCharacter = source->getChar();
    }
    auto indentString = ss.str();
    if (indentStack.top() != indentString) {
      if (indentStack.top().length() < indentString.length()) {
        indentStack.push(indentString);
        return Token(Token::TokenType::OpenBlockToken,
                     TokenVariant(indentString), current);
      } else {
        while (indentStack.top().length() > indentString.length()) {
          indentStack.pop();
        }
        if (indentStack.top().length() == indentString.length()) {
          return Token(Token::TokenType::CloseBlockToken,
                       TokenVariant(indentString), current);
        } else {
          std::string messsage =
              "Inconsistent indentation at " + current.getLinePosition();
          throw NotConsistentIndent(messsage.c_str());
        }
      }
    }
  } else if (indentStack.size() > 1) {
    indentStack.pop();
    return Token(Token::TokenType::CloseBlockToken, "", current);
  }
  return {};
}
