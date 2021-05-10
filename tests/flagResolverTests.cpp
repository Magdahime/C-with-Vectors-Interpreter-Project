#include <gtest/gtest.h>

#include "helpers/flagResolver.hpp"

TEST(FlagResolverTest, Default) {
  EXPECT_EQ(FlagResolver::Options::Help, FlagResolver::resolveOption("--help"));
  EXPECT_EQ(FlagResolver::Options::Help, FlagResolver::resolveOption("--h"));
  EXPECT_EQ(FlagResolver::Options::File, FlagResolver::resolveOption("--file"));
  EXPECT_EQ(FlagResolver::Options::File, FlagResolver::resolveOption("--f"));
  EXPECT_EQ(FlagResolver::Options::Socket, FlagResolver::resolveOption("--socket"));
  EXPECT_EQ(FlagResolver::Options::Socket, FlagResolver::resolveOption("--sc"));
  EXPECT_EQ(FlagResolver::Options::String, FlagResolver::resolveOption("--string"));
  EXPECT_EQ(FlagResolver::Options::String, FlagResolver::resolveOption("--s"));
}

TEST(FlagResolverTest, Wrong) {
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("--hellp"));
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("--hp"));
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("--ffile"));
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("--fc"));
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("--soccket"));
  EXPECT_EQ(FlagResolver::Options::Null, FlagResolver::resolveOption("1235435"));
}
