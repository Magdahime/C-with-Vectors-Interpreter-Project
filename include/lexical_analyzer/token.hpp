#pragma once
#include <variant>
#include <cstdint>
#include <string>
#include "matrix.hpp"

using TokenVariant = std::variant<int64_t, double,std::string, Matrix>;

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
        UndentifiedToken
    };

    Token(TokenType type,TokenVariant value,
        uint64_t cPos,uint64_t aPos, uint64_t lPos) 
        : type(type), value(value),
        characterPosition(cPos), absolutePosition(aPos), linePosition(lPos){}
    Token(TokenType type) :type(type) {}
    TokenType getType(){return type;}
    TokenVariant getValue() const {return value;}
    uint64_t getCharacterPosition(){return characterPosition;}
    uint64_t getAbsolutePosition(){return absolutePosition;}
    uint64_t getLinePosition(){return linePosition;}
    // friend bool operator==(const Token&, TokenType) const noexcept;
    // bool operator==(std::variant<>) const noexcept;

private:
    TokenType type;
    TokenVariant value;
    uint64_t characterPosition;
    uint64_t absolutePosition;
    uint64_t linePosition;
};


