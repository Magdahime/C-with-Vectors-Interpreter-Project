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


TEST(LexicalAnalyzerTest, detKeywordTest)
{
    StringSource src("det");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DetToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, InvKeywordTest)
{
    StringSource src("inv");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::InvToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, TransKeywordTest)
{
    StringSource src("trans");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::TransToken);
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

TEST(LexicalAnalyzerTest, someOperators2Test)
{
    StringSource src("==<=>==!!=");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::EqualToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::LessOrEqualToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::GreaterOrEqualToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AssignmentOperatorToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::NotToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::NotEqualToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, someOperators3Test)
{
    StringSource src("<<==>>");
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::LessToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::LessOrEqualToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::AssignmentOperatorToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::GreaterToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::LogicalOperatorToken);
    EXPECT_EQ(token.getSubtype(), Token::TokenSubtype::GreaterToken);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, stringLiteralsTest)
{
    std::string_view source = "'\\t\\nmamatata..43224\"\".:  '";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::StringLiteralToken);
    EXPECT_EQ(std::get<std::string>(token.getValue()).size(), source.size()-2);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, stringLiterals2Test)
{
    std::string_view source = "'                        '";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::StringLiteralToken);
    EXPECT_EQ(std::get<std::string>(token.getValue()).size(), source.size()-2);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, stringLiteralTestFAILURE)
{
    std::string_view source = "'  \n";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::StringLiteralToken);
    EXPECT_EQ(std::get<std::string>(token.getValue()).size(), source.size()-2);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, stringLiterals4Test)
{
    std::string_view source = "'                    '";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::StringLiteralToken);
    EXPECT_EQ(std::get<std::string>(token.getValue()).size(), source.size()-2);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, integerLiteralsTest)
{
    std::string_view source = "4";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IntegerLiteralToken);
    EXPECT_EQ(std::get<int64_t>(token.getValue()),4);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, integerLiterals2Test)
{
    std::string_view source = "999";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IntegerLiteralToken);
    EXPECT_EQ(std::get<int64_t>(token.getValue()),999);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, integerLiterals3Test)
{
    std::string_view source = "27";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IntegerLiteralToken);
    EXPECT_EQ(std::get<int64_t>(token.getValue()),27);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, integerLiterals4Test)
{
    std::string_view source = "5555555";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::IntegerLiteralToken);
    EXPECT_EQ(std::get<int64_t>(token.getValue()),5555555);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, doubleLiteralsTest)
{
    std::string_view source = "4.";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DoubleLiteralToken);
    EXPECT_EQ(std::get<double>(token.getValue()),4.0);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, doubleLiterals2Test)
{
    std::string_view source = "999.";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DoubleLiteralToken);
    EXPECT_EQ(std::get<double>(token.getValue()),999.0);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, doubleLiterals3Test)
{
    std::string_view source = "999.99";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DoubleLiteralToken);
    EXPECT_EQ(std::get<double>(token.getValue()), 999.99);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}

TEST(LexicalAnalyzerTest, doubleLiterals4Test)
{
    std::string_view source = "27.99777";
    StringSource src(source);
    LexicalAnalyzer lexicAna(&src);
    Token token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::DoubleLiteralToken);
    EXPECT_EQ(std::get<double>(token.getValue()),27.99777);
    token = lexicAna.getToken();
    EXPECT_EQ(token.getType(), Token::TokenType::EndOfFileToken);
}