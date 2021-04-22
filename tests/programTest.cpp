#include <gtest/gtest.h>
#include "program.hpp"
#include "source.hpp"

int number = 3;
std::vector<std::string> arguments1 = {"TKOM", "--s", "test"};
std::vector<std::string> arguments2 = {"TKOM", "--f", "test"};

TEST(ProgramTest, FileNeg)
{
    try
    {
        Program::getInstance().start(number, arguments2);
    }
    catch (CustomExceptions::Exception &ex)
    {
        EXPECT_EQ(ex.getType(), CustomExceptions::ExceptionType::StringNotAPath);
    }
}

TEST(ProgramTest, String)
{
    Program::getInstance().start(number, arguments1);
}