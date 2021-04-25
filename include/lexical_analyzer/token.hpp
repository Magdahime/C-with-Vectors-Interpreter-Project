#pragma once
#include <variant>
#include <cstdint>
#include <string>
#include "matrix.hpp"

using TokenVariant = std::variant<int64_t, double, std::string, Matrix>;

class Token
{
public:
    enum class TokenType
    {
        MatrixToken,
        IntegerToken,
        TextToken,
        DoubleToken,
        AdditiveOperatorToken,
        MultiplicativeOperatorToken,
        MatrixOperationToken,
        RelationalOperatorToken,
        LogicalOperatorToken,
        AssignmentOperatorToken,
        IfToken,
        OtherwiseToken,
        LoopToken,
        AsLongAsToken,
        FunctionToken,
        ConditionToken,
        CaseToken,
        DefaultToken,
        IdentifierToken,
        OpenRoundBracketToken,
        CloseRoundBracketToken,
        OpenSquareBracketToken,
        CloseSquareBracketToken,
        ColonToken,
        OpenBlockToken,
        CloseBlockToken,
        ApostropheToken,
        PointToken,
        VoidToken,
        ContinueToken,
        BreakToken,
        TrueToken,
        FalseToken,
        CommentToken,
        EndOfFileToken,
        NullToken,
        UnindentifiedToken,
        NextLineToken,
        AndToken,
        OrToken,
        NotToken,
        StringLiteralToken,
        IntegerLiteralToken,
        DoubleLiteralToken
    };

    enum class TokenSubtype
    {
        PlusToken,
        MinusToken,
        GreaterOrEqualToken,
        LessOrEqualToken,
        DivisionToken,
        MultiplicationToken,
    };

    Token(TokenType type, TokenVariant value,
          uint64_t cPos, uint64_t aPos, uint64_t lPos)
        : type(type), value(value),
          characterPosition(cPos), absolutePosition(aPos), linePosition(lPos) {}
    Token(TokenType type, TokenSubtype subtype, TokenVariant value,
          uint64_t cPos, uint64_t aPos, uint64_t lPos)
        : type(type), subtype(subtype), value(value),
          characterPosition(cPos), absolutePosition(aPos), linePosition(lPos) {}
    Token(TokenType type) : type(type) {}
    Token(TokenType type, TokenVariant value) : type(type), value(value) {}
    TokenType getType() { return type; }
    TokenSubtype getSubtype() { return subtype; }
    TokenVariant getValue() const{return value;}
    uint64_t getCharacterPosition() { return characterPosition; }
    uint64_t getAbsolutePosition() { return absolutePosition; }
    uint64_t getLinePosition() { return linePosition; }

private:
    TokenType type;
    TokenSubtype subtype;
    TokenVariant value;
    uint64_t characterPosition;
    uint64_t absolutePosition;
    uint64_t linePosition;

    friend bool operator==(Token const &lhs, Token const &rhs)
    {
        return lhs.type == rhs.type;
    };
};
