#pragma once
#include <stdexcept>

class Exception : public std::runtime_error
{
public:
    Exception(const char *m) : std::runtime_error(m) {}
};

class SocketProblemException : public Exception
{
public:
    SocketProblemException(const char *m) : Exception(m) {}
};
class WrongIndentException : public Exception
{
public:
    WrongIndentException(const char *m) : Exception(m) {}
};
class WrongKeywordException : public Exception
{
public:
    WrongKeywordException(const char *m) : Exception(m) {}
};
class WrongFlagsException : public Exception
{
public:
    WrongFlagsException(const char *m) : Exception(m) {}
};
class WrongFilepathException : public Exception
{
public:
    WrongFilepathException(const char *m) : Exception(m) {}
};

class CannotCreateSourceException : public Exception {
public:
    CannotCreateSourceException(const char *m) : Exception(m) {}
};

class TooLongStringLiteral : public Exception {
public:
    TooLongStringLiteral(const char *m) : Exception(m) {}
};

class WronglyDefinedStringLiteral : public Exception {
public:
    WronglyDefinedStringLiteral(const char *m) : Exception(m) {}
};

class WronglyDefinedNumberLiteral : public Exception {
public:
    WronglyDefinedNumberLiteral(const char *m) : Exception(m) {}
};

class NotConsistentIndent : public Exception {
public:
    NotConsistentIndent(const char *m) : Exception(m) {}
};