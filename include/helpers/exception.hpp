#pragma once
#include <stdexcept>

#include "interpreter/interpreterHelpers.hpp"

class Exception : public std::runtime_error {
 public:
  Exception(std::string m) : std::runtime_error(m) {}
};

class SocketProblemException : public Exception {
 public:
  SocketProblemException(std::string m) : Exception(m) {}
};
class WrongIndentException : public Exception {
 public:
  WrongIndentException(std::string m) : Exception(m) {}
};
class WrongKeywordException : public Exception {
 public:
  WrongKeywordException(std::string m) : Exception(m) {}
};
class WrongFlagsException : public Exception {
 public:
  WrongFlagsException(std::string m) : Exception(m) {}
};
class WrongFilepathException : public Exception {
 public:
  WrongFilepathException(std::string m) : Exception(m) {}
};

class CannotCreateSourceException : public Exception {
 public:
  CannotCreateSourceException(std::string m) : Exception(m) {}
};

class TooLongStringLiteral : public Exception {
 public:
  TooLongStringLiteral(std::string m) : Exception(m) {}
};

class WronglyDefinedStringLiteral : public Exception {
 public:
  WronglyDefinedStringLiteral(std::string m) : Exception(m) {}
};

class WronglyDefinedNumberLiteral : public Exception {
 public:
  WronglyDefinedNumberLiteral(std::string m) : Exception(m) {}
};

class NotConsistentIndent : public Exception {
 public:
  NotConsistentIndent(std::string m) : Exception(m) {}
};

class IntegerTooBig : public Exception {
 public:
  IntegerTooBig(std::string m) : Exception(m) {}
};

class UnexpectedToken : public Exception {
 public:
  UnexpectedToken(std::string m) : Exception(m) {}
};

class SemanticError : public Exception {
 public:
  SemanticError(std::string m) : Exception(m) {}
};

class ReturnThrow : public std::exception {
 public:
  ReturnThrow(Value newValue, int64_t newDepth)
      : value(newValue), depth(newDepth) {}

  Value getValue() const { return value; }
  int64_t getDepth() const { return depth; }

 private:
  Value value;
  int64_t depth;
};