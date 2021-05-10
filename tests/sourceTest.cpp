#include <gtest/gtest.h>
#include "source.hpp"
#include <thread>
#include "clientTCP.hpp"

std::string sample = "test";

TEST(SourceTest, openStringTest)
{
    StringSource src(sample);
    src.open();
    NextCharacter letter = src.getCurrentCharacter();
    EXPECT_EQ(letter.nextLetter, 't');
    EXPECT_EQ(letter.absolutePosition, 0);
    EXPECT_EQ(letter.characterPosition, 0);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'e');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
}

TEST(SourceTest, openFileTest)
{
    FileSource src("../tests/res/sampleText.txt");
    src.open();
    NextCharacter letter = src.getCurrentCharacter();
    EXPECT_EQ(letter.nextLetter, 'L');
    EXPECT_EQ(letter.absolutePosition, 0);
    EXPECT_EQ(letter.characterPosition, 0);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'o');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
}

TEST(SourceTest, openSocketTest)
{
    std::thread thread2([&] {
      {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
      }
        ClientTCP::run(sample.c_str(), 35555);
    });
    SocketSource src;
    thread2.join();
    src.open();
    NextCharacter letter = src.getCurrentCharacter();
    EXPECT_EQ(letter.nextLetter, 't');
    EXPECT_EQ(letter.absolutePosition, 0);
    EXPECT_EQ(letter.characterPosition, 0);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'e');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
}