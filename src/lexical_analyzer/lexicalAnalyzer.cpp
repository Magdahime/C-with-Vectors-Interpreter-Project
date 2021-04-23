#include "lexical_analyzer/lexicalAnalyzer.hpp"


void LexicalAnalyzer::initKeywordTable(){
    keywordTable["matrix"] =  Token::TokenType::MatrixToken;
    keywordTable["text"] =  Token::TokenType::TextToken;
    keywordTable["double"] =  Token::TokenType::DoubleToken;
    keywordTable["integer"] =  Token::TokenType::IntegerToken;
    keywordTable["function"] =  Token::TokenType::FunctionToken;
    keywordTable["void"] =  Token::TokenType::VoidToken;
    keywordTable["condition"] =  Token::TokenType::ConditionToken;
    keywordTable["case"] =  Token::TokenType::CaseToken;
    keywordTable["if"] =  Token::TokenType::IfToken;
    keywordTable["otherwise"] =  Token::TokenType::OtherwiseToken;
    keywordTable["loop"] =  Token::TokenType::LoopToken;
    keywordTable["asLongAs"] =  Token::TokenType::AsLongAsToken;
    keywordTable["continue"] =  Token::TokenType::ContinueToken;
    keywordTable["break"] =  Token::TokenType::BreakToken;
    keywordTable["default"] =  Token::TokenType::DefaultToken;
    keywordTable["true"] =  Token::TokenType::TrueToken;
    keywordTable["false"] =  Token::TokenType::FalseToken;
    keywordTable["#"] =  Token::TokenType::CommentToken;
    keywordTable["//"] =  Token::TokenType::CommentToken;
}