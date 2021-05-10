#pragma once
#include <map>
#include "token.hpp"

namespace LexicalTable{
const static std::map<std::string, Token::TokenType> keywordTable = {
    {"matrix", Token::TokenType::MatrixToken},
    {"text", Token::TokenType::TextToken}, 
    {"double",Token::TokenType::DoubleToken}, 
    {"integer",Token::TokenType::IntegerToken}, 
    {"function",Token::TokenType::FunctionToken}, 
    {"void",Token::TokenType::VoidToken}, 
    {"condition",Token::TokenType::ConditionToken}, 
    {"case",Token::TokenType::CaseToken}, 
    {"if",Token::TokenType::IfToken}, 
    {"otherwise",Token::TokenType::OtherwiseToken}, 
    {"loop",Token::TokenType::LoopToken}, 
    {"asLongAs",Token::TokenType::AsLongAsToken}, 
    {"continue",Token::TokenType::ContinueToken}, 
    {"break",Token::TokenType::BreakToken}, 
    {"default",Token::TokenType::DefaultToken}, 
    {"true",Token::TokenType::TrueToken}, 
    {"false",Token::TokenType::FalseToken}, 
    {"and",Token::TokenType::AndToken}, 
    {"or",Token::TokenType::OrToken}, 
    {"not",Token::TokenType::NotToken}, 
    {"det",Token::TokenType::DetToken}, 
    {"trans",Token::TokenType::TransToken}, 
    {"inv",Token::TokenType::InvToken}, 
    {"return",Token::TokenType::ReturnToken} 
};
}