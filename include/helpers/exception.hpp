#pragma once
#include <string>

class CustomExceptions
{
public:
    enum class ExceptionType
    {   
        InvalidFlagOptions,
        WrongIndent,
        WrongKeyword,
        WrongEndingOfFormula,
        UnexpectedEndOfFile,
        StringNotAPath,
        SocketProblem,
    };

    class Exception : public std::exception
    {
    public:
        Exception(ExceptionType type, const std::string &message);
        Exception(ExceptionType type, const std::string &message, std::string function, int line, int position);
        virtual ~Exception() = default;
        virtual const char *what() const noexcept override;
        ExceptionType getType(){return type;}
    private:
        ExceptionType type;
        std::string message;
    };
};