#include "lexical_analyzer/lexicalAnalyzer.hpp"

Token LexicalAnalyzer::getToken()
{
    Token nextToken = buildEOF();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    if (!isNextLine)
        skipWhites();
    else
    {
        nextToken = buildIndent();
        if (nextToken.getType() != Token::TokenType::NullToken)
        {
            isNextLine = false;
            return nextToken;
        }else{
            isNextLine = false;
        }
    }
    nextToken = buildStringLiteral();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;
    nextToken = buildNumber();
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
    nextToken = buildLogicalOperatorToken();
    if (nextToken.getType() != Token::TokenType::NullToken)
        return nextToken;

    return buildUnindentified();
}

Token LexicalAnalyzer::buildIdentifierOrKeyword()
{
    NextCharacter current = source->getCurrentCharacter();
    if (isalpha(current.nextLetter))
    {
        std::stringstream ss;
        ss << current.nextLetter;
        NextCharacter nextCharacter = source->getChar();
        while (isalnum(nextCharacter.nextLetter) || nextCharacter.nextLetter == '_')
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
               nextCharacter.nextLetter != '\0')
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
            ++length;
        }
        if (length >= MAXSIZE)
        {
            std::string message = "Commentary at " + current.getLinePosition() + " is too long.";
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
                std::string message = "Commentary at " + current.getLinePosition() + " is too long.";
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
        isNextLine = true;
        return Token(Token::TokenType::NextLineToken, TokenVariant(""),
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
    case (','):
        source->getChar();
        return Token(Token::TokenType::CommaToken, TokenVariant(","),
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
        while (length < MAXSIZE && isprint(nextCharacter.nextLetter) && nextCharacter.nextLetter != '\n' && nextCharacter.nextLetter != delimiter && nextCharacter.nextLetter != '\0')
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
            length++;
        }
        if (length >= MAXSIZE)
        {
            std::string message = "String literal at " + current.getLinePosition() + " is too long.";
            throw TooLongStringLiteral(message.c_str());
        }
        else if (nextCharacter.nextLetter == delimiter)
        {
            source->getChar();
            return Token(Token::TokenType::StringLiteralToken,
                         TokenVariant(ss.str()),
                         current.characterPosition,
                         current.absolutePosition,
                         current.linePosition);
        }
        else
        {
            std::string message = "String literal at " + current.getLinePosition() + " is malformed.";
            throw WronglyDefinedStringLiteral(message.c_str());
        }
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildNumber()
{
    NextCharacter current = source->getCurrentCharacter();
    if (isdigit(current.nextLetter))
    {
        int64_t integerToBe = current.nextLetter - '0';
        uint64_t base = 10;
        NextCharacter nextCharacter = source->getChar();
        while (isdigit(nextCharacter.nextLetter))
        {
            integerToBe *= base;
            integerToBe += nextCharacter.nextLetter - '0';
            nextCharacter = source->getChar();
        }
        if (nextCharacter.nextLetter == '.')
        {
            nextCharacter = source->getChar();
            double doubleToBe = integerToBe;
            double doubleBase = 0.1;
            while (isdigit(nextCharacter.nextLetter))
            {
                doubleToBe += doubleBase * (nextCharacter.nextLetter - '0');
                doubleBase *= 0.1;
                nextCharacter = source->getChar();
            }
            return Token(Token::TokenType::DoubleLiteralToken,
                         TokenVariant(doubleToBe), current.characterPosition,
                         current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::IntegerLiteralToken,
                         TokenVariant(integerToBe), current.characterPosition,
                         current.absolutePosition, current.linePosition);
        }
    }
    return Token(Token::TokenType::NullToken);
}

Token LexicalAnalyzer::buildLogicalOperatorToken()
{
    NextCharacter current = source->getCurrentCharacter();
    switch (current.nextLetter)
    {
    case ('<'):
    {
        NextCharacter nextCharacter = source->getChar();
        if (nextCharacter.nextLetter == '=')
        {
            source->getChar();
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::LessOrEqualToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::LessToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        break;
    }

    case ('>'):
    {
        NextCharacter nextCharacter = source->getChar();
        if (nextCharacter.nextLetter == '=')
        {
            source->getChar();
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::GreaterOrEqualToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::GreaterToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        break;
    }

    case ('='):
    {
        NextCharacter nextCharacter = source->getChar();
        if (nextCharacter.nextLetter == '=')
        {
            source->getChar();
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::EqualToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::AssignmentOperatorToken,
                         TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        break;
    }

    case ('!'):
    {
        NextCharacter nextCharacter = source->getChar();
        if (nextCharacter.nextLetter == '=')
        {
            source->getChar();
            return Token(Token::TokenType::LogicalOperatorToken,
                         Token::TokenSubtype::NotEqualToken, TokenVariant(""),
                         current.characterPosition, current.absolutePosition, current.linePosition);
        }
        else
        {
            return Token(Token::TokenType::NotToken,
                         TokenVariant(""), current.characterPosition,
                         current.absolutePosition, current.linePosition);
        }
        break;
    }
    }
    return Token(Token::TokenType::NullToken);
}

void LexicalAnalyzer::skipWhites()
{
    NextCharacter current = source->getCurrentCharacter();
    if (isspace(current.nextLetter) && current.nextLetter != '\n')
    {
        NextCharacter nextCharacter = source->getChar();
        while (isspace(nextCharacter.nextLetter) && current.nextLetter != '\n')
        {
            nextCharacter = source->getChar();
        }
    }
}

Token LexicalAnalyzer::buildIndent()
{
    NextCharacter current = source->getCurrentCharacter();
    if (current.nextLetter == ' ' || current.nextLetter == '\t')
    {
        if (chosenIndentChar == 0)
        {
            chosenIndentChar = current.nextLetter;
        }
        else if (current.nextLetter != chosenIndentChar)
        {
            std::string messsage = "Inconsistent use of tabs and spaces in indentation at " +
                                   current.getLinePosition();
            throw NotConsistentIndent(messsage.c_str());
        }
        std::stringstream ss;
        ss << current.nextLetter;
        NextCharacter nextCharacter = source->getChar();
        while (nextCharacter.nextLetter == chosenIndentChar)
        {
            ss << nextCharacter.nextLetter;
            nextCharacter = source->getChar();
        }
        auto indentString = ss.str();
        if (indentStack.top() != indentString)
        {
            if (indentStack.top().length() < indentString.length())
            {
                indentStack.push(indentString);
                return Token(Token::TokenType::OpenBlockToken, TokenVariant(indentString),
                current.characterPosition, current.absolutePosition, current.linePosition);
            }
            else
            {
                while (indentStack.top().length() > indentString.length())
                {
                    indentStack.pop();
                }
                if (indentStack.top().length() == indentString.length())
                {
                return Token(Token::TokenType::CloseBlockToken, TokenVariant(indentString),
                    current.characterPosition, current.absolutePosition, current.linePosition);   
                }
                else
                {
                    std::string messsage = "Inconsistent indentation at " +
                                           current.getLinePosition();
                    throw NotConsistentIndent(messsage.c_str());
                }
            }
        }
    }
    return Token(Token::TokenType::NullToken);
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
    keywordTable["det"] = Token::TokenType::DetToken;
    keywordTable["trans"] = Token::TokenType::TransToken;
    keywordTable["inv"] = Token::TokenType::InvToken;
    keywordTable["return"] = Token::TokenType::ReturnToken;
}