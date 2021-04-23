#pragma once
#include <variant>
#include <cstdint>
#include <string>
#include "matrix.hpp"
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
    };

    Token(TokenType type,std::variant<int, double,std::string, Matrix> value,
        uint64_t cPos,uint64_t aPos, uint64_t lPos) 
        : type(type), value(value),
        characterPosition(cPos), absolutePosition(aPos), linePosition(lPos){}
    TokenType getType(){return type;}
    std::variant<int, double,std::string, Matrix> getValue(){return value;}
    uint64_t getCharacterPosition(){return characterPosition;}
    uint64_t getAbsolutePosition(){return absolutePosition;}
    uint64_t getLinePosition(){return linePosition;}

private:
    TokenType type;
    std::variant<int, double,std::string, Matrix> value;
    uint64_t characterPosition;
    uint64_t absolutePosition;
    uint64_t linePosition;
};
