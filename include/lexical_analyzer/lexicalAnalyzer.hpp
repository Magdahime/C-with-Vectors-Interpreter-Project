#include <map>
#include "token.hpp"
#include "source.hpp"
class LexicalAnalyzer
{
public:
    LexicalAnalyzer(SourceBase* source):source(source){};
    Token getToken();
    Token buildNumber();
    Token buildIdentifierOrKeyword();
    Token buildOperator();
    Token buildConstString();
    Token buildMatrix();
    Token buildComment();
private:
    Token buildInteger();
    Token buildFloatingPoint();
    SourceBase* source;
    std::map<std::string, Token::TokenType> keywordTable;
    void initKeywordTable();
};

using LexicalAnalyzerUptr = std::unique_ptr<LexicalAnalyzer>;