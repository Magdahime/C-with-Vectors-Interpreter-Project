#include <map>
#include <stack>
#include "token.hpp"
#include "source.hpp"
class LexicalAnalyzer
{
public:
    LexicalAnalyzer(SourceBase* source):source(source){
        source->open();
        initKeywordTable();
        isNextLine = true;
        chosenIndentChar = 0;
        indentStack.push("");
    };
    Token getToken();

private:
    void skipWhites();
    Token buildNumber();
    Token buildIdentifierOrKeyword();
    Token buildDivisionTokenOrComment();
    Token buildStringLiteral();
    Token buildComment();
    Token buildIndent();
    Token buildUnindentified();
    Token buildLogicalOperatorToken();
    Token buildEOF();
    Token buildOneCharToken();
    Token buildIndentToken();
    SourceBase* source;
    std::map<std::string, Token::TokenType> keywordTable;
    bool isNextLine;
    char chosenIndentChar;
    std::stack<std::string> indentStack;
    void initKeywordTable();
    const uint32_t MAXSIZE = 2048;
};

using LexicalAnalyzerUptr = std::unique_ptr<LexicalAnalyzer>;