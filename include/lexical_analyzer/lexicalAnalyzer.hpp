#include <map>
#include "token.hpp"
#include "source.hpp"
class LexicalAnalyzer
{
public:
    LexicalAnalyzer(SourceBase* source):source(source){
        source->open();
        initKeywordTable();
    };
    Token getToken();

private:
    Token buildNumber();
    Token buildIdentifierOrKeyword();
    Token buildDivisionTokenOrComment();
    Token buildConstString();
    Token buildMatrix();
    Token buildComment();
    Token buildUnindentified();
    Token buildLogicalOperatorToken();
    Token buildEOF();
    Token buildOneCharToken();
    Token buildIndentToken();
    SourceBase* source;
    std::map<std::string, Token::TokenType> keywordTable;
    void initKeywordTable();
    const uint32_t MAXSIZE = 2048;
};

using LexicalAnalyzerUptr = std::unique_ptr<LexicalAnalyzer>;