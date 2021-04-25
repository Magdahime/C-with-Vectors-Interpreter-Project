#include "lexical_analyzer/lexicalAnalyzer.hpp"

Token LexicalAnalyzer::getToken()
{
    Token nextToken = buildEOF();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildStringLiteral();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildIdentifierOrKeyword();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildComment();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildDivisionTokenOrComment();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildOneCharToken();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    return buildUnindentified();
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
    keywordTable["and"] = Token::TokenType::AndToken;
    keywordTable["or"] = Token::TokenType::OrToken;
    keywordTable["not"] = Token::TokenType::NotToken;
}

Token LexicalAnalyzer::buildIdentifierOrKeyword()
{
    NextCharacter current = source->getCurrentCharacter();
    if (isalpha(current.nextLetter))
    {
        std::stringstream ss;
        ss << current.nextLetter;
        NextCharacter nextCharacter = source->getChar();
        while (isalpha(nextCharacter.nextLetter))
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
        }
        auto type = keywordTable.find(ss.str());
        if (type != keywordTable.end())
            return Token(type->second, TokenVariant(ss.str()),
                         current.characterPosition, current.absolutePosition,
                         current.linePosition);
        else
            return Token(Token::TokenType::IdentifierToken, TokenVariant(ss.str()),
                         current.characterPosition, current.absolutePosition, current.linePosition);
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildEOF()
{
    NextCharacter current = source->getCurrentCharacter();
    if (current.nextLetter == '\0')
    {
        return Token(Token::TokenType::EndOfFileToken, TokenVariant(""),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildComment()
{
    NextCharacter current = source->getCurrentCharacter();
    if (current.nextLetter == '#')
    {
        std::stringstream ss;
        ss << current.nextLetter;
        uint32_t length = 1;
        NextCharacter nextCharacter = source->getChar();
        while (length <= MAXSIZE && nextCharacter.nextLetter != '\n' &&
               nextCharacter.nextLetter != '\0' )
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
            ++length;
        }
        if (length >= MAXSIZE)
        {
            std::string message = "Commentary at " + std::to_string(current.characterPosition) + " : " + std::to_string(current.linePosition) + " is too long.";
            throw TooLongStringLiteral(message.c_str());
        }
        return Token(Token::TokenType::CommentToken, TokenVariant(ss.str()),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildUnindentified()
{
    NextCharacter current = source->getCurrentCharacter();
    return Token(Token::TokenType::UnindentifiedToken, TokenVariant(""),
                 current.characterPosition, current.absolutePosition, current.linePosition);
}

Token LexicalAnalyzer::buildDivisionTokenOrComment()
{
    NextCharacter current = source->getCurrentCharacter();
    if (current.nextLetter == '/')
    {
        std::stringstream ss;
        ss << current.nextLetter;
        NextCharacter nextCharacter = source->getChar();
        if (current.nextLetter == '/')
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
            uint32_t length = 2;
            while (length < MAXSIZE && 
            nextCharacter.nextLetter != '\n' && nextCharacter.nextLetter != '\0')
            {
                ss << nextCharacter.nextLetter;
                nextCharacter = source->getChar();
                ++length;
            }
            if (length >= MAXSIZE)
            {
                std::string message = "Commentary at " + std::to_string(current.characterPosition) + " : " + std::to_string(current.linePosition) + " is too long.";
                throw TooLongStringLiteral(message.c_str());
            }

            return Token(Token::TokenType::CommentToken, TokenVariant(ss.str()),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::MultiplicativeOperatorToken,
                         Token::TokenSubtype::DivisionToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildOneCharToken()
{
    NextCharacter current = source->getCurrentCharacter();
    switch (current.nextLetter)
    {
    case ('+'):
        source->getChar();
        return Token(Token::TokenType::AdditiveOperatorToken, Token::TokenSubtype::PlusToken,
                     TokenVariant("+"),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case ('\n'):
        source->getChar();
        return Token(Token::TokenType::NextLineToken, TokenVariant("\n"),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case ('-'):
        source->getChar();
        return Token(Token::TokenType::AdditiveOperatorToken, Token::TokenSubtype::MinusToken,
                     TokenVariant("-"),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case ('('):
        source->getChar();
        return Token(Token::TokenType::OpenRoundBracketToken, TokenVariant("("),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case (')'):
        source->getChar();
        return Token(Token::TokenType::CloseRoundBracketToken, TokenVariant("("),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case ('['):
        source->getChar();
        return Token(Token::TokenType::OpenSquareBracketToken, TokenVariant("["),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case (']'):
        source->getChar();
        return Token(Token::TokenType::CloseSquareBracketToken, TokenVariant("]"),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case (':'):
        source->getChar();
        return Token(Token::TokenType::ColonToken, TokenVariant(":"),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    case ('.'):
        source->getChar();
        return Token(Token::TokenType::PointToken, TokenVariant("."),
                     current.characterPosition, current.absolutePosition, current.linePosition);
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildStringLiteral()
{
    NextCharacter current = source->getCurrentCharacter();
    if (current.nextLetter == '\'' || current.nextLetter == '\"')
    {
        char delimiter = current.nextLetter;
        std::stringstream ss;
        NextCharacter nextCharacter = source->getChar();
        uint32_t length = 1;
        while (length < MAXSIZE && isprint(nextCharacter.nextLetter) 
                && nextCharacter.nextLetter != '\n' && nextCharacter.nextLetter != delimiter
                && nextCharacter.nextLetter != '\0')
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
            length++;
        }
        if (length >= MAXSIZE){
                std::string message = "String literal at " 
                + std::to_string(current.characterPosition) 
                + " : " + std::to_string(current.linePosition) 
                + " is too long.";
                throw TooLongStringLiteral(message.c_str());
        }else if(nextCharacter.nextLetter == delimiter){
            source->getChar();
            return Token(Token::TokenType::StringLiteralToken,
                    TokenVariant(ss.str()), 
                    current.characterPosition, 
                    current.absolutePosition,
                    current.linePosition);
        }else{
            std::string message = "String literal at " 
                + std::to_string(current.characterPosition) 
                + " : " + std::to_string(current.linePosition) 
                + " is malformed.";
            throw WronglyDefinedStringLiteral(message.c_str());
        }
    }
    return Token(Token::TokenType::NullToken);
}
