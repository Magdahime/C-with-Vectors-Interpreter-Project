#pragma once
#include <string>

class CustomExceptions
{
public:
    enum class ExceptionType
    {
        WrongIndent,
        WrongKeyword,
        WrongEndingOfFormula,
        UnexpectedEndOfFile,
        StringNotAPath,
    };

    class Exception : public std::exception
    {
    public:
        Exception(ExceptionType type, const std::string &message);
        Exception(ExceptionType type, const std::string &message, std::string function, int line, int position);
        virtual ~Exception() = default;
        virtual const char *what() const noexcept override;

    private:
        ExceptionType type;
        std::string message;
    };
};