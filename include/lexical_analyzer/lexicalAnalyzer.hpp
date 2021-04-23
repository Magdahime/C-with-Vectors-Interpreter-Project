#include <map>
#include "token.hpp"
class LexicalAnalyzer{
    public:

    private:
    std::map<std::string, Token::TokenType> keywordTable;
    void initKeywordTable();
};