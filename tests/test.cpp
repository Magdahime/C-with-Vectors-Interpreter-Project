#include <gtest/gtest.h> // googletest header file

#include "exception.hpp"
using std::string;

const char *actualValTrue  = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal      = "hello gtest";

TEST(ExceptionsTests, CStrEqual) {
    EXPECT_STREQ(expectVal, actualValTrue);
}

TEST(ExceptionsTests, CStrNotEqual) {
    EXPECT_STREQ(expectVal, actualValFalse);
}