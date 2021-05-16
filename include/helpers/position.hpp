#pragma once
#include <memory>
class Position {
 public:
  constexpr Position(uint64_t absolutePosition, uint64_t line,
                     uint64_t character)
      : absolutePosition(absolutePosition), line(line), character(character){};
  constexpr Position() : absolutePosition(0), line(0), character(0){};
  constexpr Position nextChar() const {
    return Position(this->absolutePosition + 1, this->line,
                    this->character + 1);
  }
  constexpr Position nextLine() const {
    return Position(this->absolutePosition, this->line + 1, 0);
  }
  constexpr uint64_t getChar() const { return character; }
  constexpr uint64_t getLine() const { return line; }
  constexpr uint64_t getAbsolutePosition() const { return absolutePosition; }

 private:
  uint64_t absolutePosition;
  uint64_t line;
  uint64_t character;
};