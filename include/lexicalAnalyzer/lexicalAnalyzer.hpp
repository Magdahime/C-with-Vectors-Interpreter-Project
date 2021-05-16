#pragma once
#include <cmath>
#include <limits>
#include <map>
#include <optional>
#include <stack>

#include "helpers/operators.hpp"
#include "lexicalTable.hpp"
#include "source.hpp"
#include "token.hpp"
class LexicalAnalyzer {
 public:
  LexicalAnalyzer(SourceBase* source)
    : source(source), isNextLine(true), chosenIndentChar(0) {
    source->open();
    indentStack.push("");
  };
  void setSource(SourceBase* source) {
    this->source = source;
    source->open();
  }
  std::optional<Token> getToken();

 private:
  void skipWhites();
  std::optional<Token> buildNumber();
  int64_t buildInteger(NextCharacter& current);
  double buildFloatingPointNumber(NextCharacter& current, int64_t integerPart);
  bool checkIfFits(const std::string_view limit,
                   const std::string_view numberToCheck) const;
  std::string checkForEscapedSequence(NextCharacter& current) const;
  std::optional<Token> buildIdentifierOrKeyword();
  std::optional<Token> buildDivisionTokenOrComment();
  std::optional<Token> buildStringLiteral();
  std::optional<Token> buildComment();
  std::optional<Token> buildIndent();
  std::optional<Token> buildUnindentified();
  std::optional<Token> buildLogicalOperatorToken();
  std::optional<Token> buildEOF();
  std::optional<Token> buildOneCharToken();
  std::optional<Token> buildIndentToken();
  SourceBase* source;
  bool isNextLine;
  char chosenIndentChar;
  std::stack<std::string> indentStack;
  const uint32_t MAXSIZE = 2048;
};

using LexicalAnalyzerUptr = std::unique_ptr<LexicalAnalyzer>;