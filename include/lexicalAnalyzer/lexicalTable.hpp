#pragma once
#include <unordered_map>

#include "token.hpp"

namespace LexicalTable {
const static std::unordered_map<std::string, Token::TokenType> keywordTable = {
    {"matrix", Token::TokenType::MatrixToken},
    {"text", Token::TokenType::TextToken},
    {"double", Token::TokenType::DoubleToken},
    {"integer", Token::TokenType::IntegerToken},
    {"function", Token::TokenType::FunctionToken},
    {"void", Token::TokenType::VoidToken},
    {"condition", Token::TokenType::ConditionToken},
    {"case", Token::TokenType::CaseToken},
    {"if", Token::TokenType::IfToken},
    {"otherwise", Token::TokenType::OtherwiseToken},
    {"loop", Token::TokenType::LoopToken},
    {"asLongAs", Token::TokenType::AsLongAsToken},
    {"default", Token::TokenType::DefaultToken},
    {"and", Token::TokenType::AndToken},
    {"or", Token::TokenType::OrToken},
    {"not", Token::TokenType::NotToken},
    {"det", Token::TokenType::DetToken},
    {"trans", Token::TokenType::TransToken},
    {"inv", Token::TokenType::InvToken},
    {"return", Token::TokenType::ReturnToken}};

const static std::unordered_map<std::string, char> escapeTable = {
    {"\\n", '\n'},  {"\\t", '\t'}, {"\\'", '\''},
    {"\\\"", '\"'}, {"\\?", '?'},  {"\\\\", '\\'}};

const static std::unordered_map<Token::TokenType, std::string>
    token2StringTable{
        {Token::TokenType::MatrixToken, "MatrixToken"},
        {Token::TokenType::IntegerToken, "IntegerToken"},
        {Token::TokenType::TextToken, "TextToken"},
        {Token::TokenType::DoubleToken, "DoubleToken"},
        {Token::TokenType::AdditiveOperatorToken, "AdditiveOperatorToken"},
        {Token::TokenType::MultiplicativeOperatorToken,
         "MultiplicativeOperatorToken"},
        {Token::TokenType::ExponentiationOperatorToken,
         "ExponentiationOperatorToken"},
        {Token::TokenType::LogicalOperatorToken, "LogicalOperatorToken"},
        {Token::TokenType::AssignmentOperatorToken, "AssignmentOperatorToken"},
        {Token::TokenType::IfToken, "IfToken"},
        {Token::TokenType::OtherwiseToken, "OtherwiseToken"},
        {Token::TokenType::LoopToken, "LoopToken"},
        {Token::TokenType::AsLongAsToken, "AsLongAsToken"},
        {Token::TokenType::FunctionToken, "FunctionToken"},
        {Token::TokenType::ConditionToken, "ConditionToken"},
        {Token::TokenType::CaseToken, "CaseToken"},
        {Token::TokenType::IdentifierToken, "IdentifierToken"},
        {Token::TokenType::OpenRoundBracketToken, "OpenRoundBracketToken"},
        {Token::TokenType::CloseRoundBracketToken, "CloseRoundBracketToken"},
        {Token::TokenType::OpenSquareBracketToken, "OpenSquareBracketToken"},
        {Token::TokenType::CloseSquareBracketToken, "CloseSquareBracketToken"},
        {Token::TokenType::ColonToken, "ColonToken"},
        {Token::TokenType::OpenBlockToken, "OpenBlockToken"},
        {Token::TokenType::CloseBlockToken, "CloseBlockToken"},
        {Token::TokenType::CommaToken, "CommaToken"},
        {Token::TokenType::PointToken, "PointToken"},
        {Token::TokenType::VoidToken, "VoidToken"},
        {Token::TokenType::DefaultToken, "DefaultToken"},
        {Token::TokenType::CommentToken, "CommentToken"},
        {Token::TokenType::RootToken, "RootToken"},
        {Token::TokenType::EndOfFileToken, "EndOfFileToken"},
        {Token::TokenType::UnindentifiedToken, "UnindentifiedToken"},
        {Token::TokenType::NextLineToken, "NextLineToken"},
        {Token::TokenType::AndToken, "AndToken"},
        {Token::TokenType::OrToken, "OrToken"},
        {Token::TokenType::NotToken, "NotToken"},
        {Token::TokenType::StringLiteralToken, "StringLiteralToken"},
        {Token::TokenType::IntegerLiteralToken, "IntegerLiteralToken"},
        {Token::TokenType::DoubleLiteralToken, "DoubleLiteralToken"},
        {Token::TokenType::MatrixLiteralToken, "MatrixLiteralToken"},
        {Token::TokenType::DetToken, "DetToken"},
        {Token::TokenType::TransToken, "TransToken"},
        {Token::TokenType::InvToken, "InvToken"},
        {Token::TokenType::ReturnToken, "ReturnToken"}};

}  // namespace LexicalTable