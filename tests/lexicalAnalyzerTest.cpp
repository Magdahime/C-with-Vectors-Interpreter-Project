#include <gtest/gtest.h>
#include "source.hpp"
#include "lexical_analyzer/token.hpp"
#include "lexical_analyzer/lexicalAnalyzer.hpp"
TEST(LexicalAnalyzerTest, matrixKeywordTest)
{
    StringSource src("matrix");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::MatrixToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, textKeywordTest)
{
    StringSource src("text");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::TextToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, doubleKeywordTest)
{
    StringSource src("double");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DoubleToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, integerKeywordTest)
{
    StringSource src("integer");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IntegerToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, functionKeywordTest)
{
    StringSource src("function");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::FunctionToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, voidKeywordTest)
{
    StringSource src("void");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::VoidToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, conditionKeywordTest)
{
    StringSource src("condition");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::ConditionToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, caseKeywordTest)
{
    StringSource src("case");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CaseToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, ifKeywordTest)
{
    StringSource src("if");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IfToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, otherwiseKeywordTest)
{
    StringSource src("otherwise");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::OtherwiseToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, loopKeywordTest)
{
    StringSource src("loop");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LoopToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, asLongAsKeywordTest)
{
    StringSource src("asLongAs");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AsLongAsToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, continueKeywordTest)
{
    StringSource src("continue");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::ContinueToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, breakKeywordTest)
{
    StringSource src("break");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::BreakToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, defaultKeywordTest)
{
    StringSource src("default");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DefaultToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, trueKeywordTest)
{
    StringSource src("true");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::TrueToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, andKeywordTest)
{
    StringSource src("and");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AndToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, orKeywordTest)
{
    StringSource src("or");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::OrToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, notKeywordTest)
{
    StringSource src("not");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::NotToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, falseKeywordTest)
{
    StringSource src("false");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::FalseToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, commentKeywordTest)
{
    StringSource src("#To jest komentarz\n");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CommentToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::NextLineToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, commentKeyword2Test)
{
    StringSource src("//To też jest komentarz\n");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CommentToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::NextLineToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, commentWithoutNewline1Test)
{
    StringSource src("//To też jest komentarz");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CommentToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, commentWithoutNewline2Test)
{
    StringSource src("#To też jest komentarz");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CommentToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, identifierKeywordTest)
{
    StringSource src("bunny");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IdentifierToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, openCloseBracketsTest)
{
    StringSource src("()[]");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::OpenRoundBracketToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CloseRoundBracketToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::OpenSquareBracketToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::CloseSquareBracketToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, someOperatorsTest)
{
    StringSource src(".+-:");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::PointToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AdditiveOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::PlusToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AdditiveOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::MinusToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::ColonToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}