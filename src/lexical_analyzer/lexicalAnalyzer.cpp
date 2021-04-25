#include "lexical_analyzer/lexicalAnalyzer.hpp"

Token LexicalAnalyzer::getToken()
{
    NextCharacter current = source->getCurrentCharacter();
    Token nextToken = buildIdentifierOrKeyword();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    return Token(Token::TokenType::UndentifiedToken, TokenVariant(""),
                 current.characterPosition, current.absolutePosition, current.linePosition);
}

void LexicalAnalyzer::initKeywordTable()
{
    keywordTable["matrix"] = Token::TokenType::MatrixToken;
    keywordTable["text"] = Token::TokenType::TextToken;
    keywordTable["double"] = Token::TokenType::DoubleToken;
    keywordTable["integer"] = Token::TokenType::IntegerToken;
    keywordTable["function"] = Token::TokenType::FunctionToken;
    keywordTable["void"] = Token::TokenType::VoidToken;
    keywordTable["condition"] = Token::TokenType::ConditionToken;
    keywordTable["case"] = Token::TokenType::CaseToken;
    keywordTable["if"] = Token::TokenType::IfToken;
    keywordTable["otherwise"] = Token::TokenType::OtherwiseToken;
    keywordTable["loop"] = Token::TokenType::LoopToken;
    keywordTable["asLongAs"] = Token::TokenType::AsLongAsToken;
    keywordTable["continue"] = Token::TokenType::ContinueToken;
    keywordTable["break"] = Token::TokenType::BreakToken;
    keywordTable["default"] = Token::TokenType::DefaultToken;
    keywordTable["true"] = Token::TokenType::TrueToken;
    keywordTable["false"] = Token::TokenType::FalseToken;
    keywordTable["#"] = Token::TokenType::CommentToken;
    keywordTable["//"] = Token::TokenType::CommentToken;
}

// Token LexicalAnalyzer::buildInteger(){
//     if(isdigit(source->getCurrentCharacter())){
//         uint64_t integerToBe = source->getCurrentCharacter();
//         uint64_t base = 10;
//         NextCharacter nextCharacter = source->getChar();
//         while(isdigit(nextCharacter.nextLetter)){
//             integerToBe += base * nextCharacter.nextLetter;
//         }
//         if(isspace(source->getCurrentCharacter())){

//         }
//     }
//     return Token(Token::TokenType::NullToken);
// }

Token LexicalAnalyzer::buildIdentifierOrKeyword()
{
    NextCharacter currentCharacter = source->getCurrentCharacter();
    if (isalpha(currentCharacter.nextLetter))
    {
        std::stringstream ss;
        ss << currentCharacter.nextLetter;
        NextCharacter nextCharacter = source->getChar();
        while (isalpha(nextCharacter.nextLetter))
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
        }
        auto type = keywordTable.find(ss.str());
        if (type != keywordTable.end())
            return Token(type->second, TokenVariant(ss.str()),
                         currentCharacter.characterPosition, currentCharacter.absolutePosition,
                         currentCharacter.linePosition);
        else
            return Token(Token::TokenType::IdentifierToken, TokenVariant(ss.str()),
                         currentCharacter.characterPosition, currentCharacter.absolutePosition, currentCharacter.linePosition);
    }
    return Token(Token::TokenType::NullToken);
}