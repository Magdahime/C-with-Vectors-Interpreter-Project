#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include <unordered_map>

#include "matrix.hpp"
#include "source.hpp"

using TokenVariant =
    std::variant<std::monostate, int64_t, double, std::string, Matrix>;

class Token {
 public:
  enum class TokenType {
    MatrixToken,
    IntegerToken,
    TextToken,
    DoubleToken,
    AdditiveOperatorToken,
    MultiplicativeOperatorToken,
    ExponentiationOperatorToken,
    LogicalOperatorToken,
    AssignmentOperatorToken,
    IfToken,
    OtherwiseToken,
    LoopToken,
    AsLongAsToken,
    FunctionToken,
    ConditionToken,
    CaseToken,
    IdentifierToken,
    OpenRoundBracketToken,
    CloseRoundBracketToken,
    OpenSquareBracketToken,
    CloseSquareBracketToken,
    ColonToken,
    OpenBlockToken,
    CloseBlockToken,
    CommaToken,
    PointToken,
    VoidToken,
    ContinueToken,
    BreakToken,
    TrueToken,
    FalseToken,
    DefaultToken,
    CommentToken,
    RootToken,
    EndOfFileToken,
    UnindentifiedToken,
    NextLineToken,
    AndToken,
    OrToken,
    NotToken,
    StringLiteralToken,
    IntegerLiteralToken,
    DoubleLiteralToken,
    DetToken,
    TransToken,
    InvToken,
    ReturnToken,
  };

  enum class TokenSubtype {
    PlusToken,
    MinusToken,
    GreaterOrEqualToken,
    LessOrEqualToken,
    LessToken,
    GreaterToken,
    EqualToken,
    NotEqualToken,
    DivisionToken,
    MultiplicationToken,
  };

  Token(TokenType type, TokenVariant value, const NextCharacter &firstCharacter)
      : type(type),
        value(value),
        characterPosition(firstCharacter.characterPosition),
        absolutePosition(firstCharacter.absolutePosition),
        linePosition(firstCharacter.linePosition) {}
  Token(TokenType type, TokenSubtype subtype, TokenVariant value,
        const NextCharacter &firstCharacter)
      : type(type),
        subtype(subtype),
        value(value),
        characterPosition(firstCharacter.characterPosition),
        absolutePosition(firstCharacter.absolutePosition),
        linePosition(firstCharacter.linePosition) {}
  Token(TokenType type) : type(type) {}
  Token(TokenType type, TokenVariant value) : type(type), value(value) {}
  TokenType getType() const { return type; }
  TokenSubtype getSubtype() const { return subtype; }

  TokenVariant getValue() const { return value; }
  int64_t getInt() const { return std::get<int64_t>(value); }
  double  getDouble() const { return std::get<double>(value); }
  std::string getString() const {return std::get<std::string>(value); }

  uint64_t getCharacterPosition() const { return characterPosition; }
  uint64_t getAbsolutePosition() const { return absolutePosition; }
  uint64_t getLinePosition() const { return linePosition; }
  std::string getLinePositionString() const {
    return std::to_string(linePosition+1) + ":" +
           std::to_string(characterPosition+1);
  }

 private:
  TokenType type;
  TokenSubtype subtype;
  TokenVariant value;
  uint64_t characterPosition;
  uint64_t absolutePosition;
  uint64_t linePosition;

  friend bool operator==(Token const &lhs, Token const &rhs) {
    return lhs.type == rhs.type && lhs.value == rhs.value;
  };

  friend bool operator!=(Token const &lhs, Token const &rhs) {
    return !operator==(rhs, lhs);
  };

  friend bool operator==(Token const &lhs, Token::TokenType const &rhs) {
    return lhs.type == rhs;
  };

  friend bool operator==(Token::TokenType const &lhs, Token const &rhs) {
    return lhs == rhs.type;
  };
};
