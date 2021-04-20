#include <gtest/gtest.h>

#include "helpers/flagResolver.hpp"

TEST(FlagResolverTest, Default) {
    FlagResolver fr;
  EXPECT_EQ(FlagResolver::Options::Help, fr.resolveOption("--help"));
  EXPECT_EQ(FlagResolver::Options::Help, fr.resolveOption("--h"));
  EXPECT_EQ(FlagResolver::Options::File, fr.resolveOption("--file"));
  EXPECT_EQ(FlagResolver::Options::File, fr.resolveOption("--f"));
  EXPECT_EQ(FlagResolver::Options::Socket, fr.resolveOption("--socket"));
  EXPECT_EQ(FlagResolver::Options::Socket, fr.resolveOption("--sc"));
  EXPECT_EQ(FlagResolver::Options::String, fr.resolveOption("--string"));
  EXPECT_EQ(FlagResolver::Options::String, fr.resolveOption("--s"));
}

TEST(FlagResolverTest, Wrong) {
    FlagResolver fr;
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("--hellp"));
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("--hp"));
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("--ffile"));
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("--fc"));
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("--soccket"));
  EXPECT_EQ(FlagResolver::Options::Null, fr.resolveOption("1235435"));
}
