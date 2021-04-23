#include <gtest/gtest.h>
#include "source.hpp"
#include <thread>
#include "clientTCP.hpp"

std::string sample = "test";

TEST(SourceTest, openStringTest)
{
    Source src;
    src.openString(sample);
    Source::NextCharacter letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 't');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'e');
    EXPECT_EQ(letter.absolutePosition, 2);
    EXPECT_EQ(letter.characterPosition, 2);
    EXPECT_EQ(letter.linePosition, 0);
}

TEST(SourceTest, openFileTest)
{
    Source src;
    src.openFile("../tests/res/sampleText.txt");
    Source::NextCharacter letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'L');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'o');
    EXPECT_EQ(letter.absolutePosition, 2);
    EXPECT_EQ(letter.characterPosition, 2);
    EXPECT_EQ(letter.linePosition, 0);
}

TEST(SourceTest, openSocketTest)
{
    SocketWrapper sw;
    std::thread thread1([&] { sw.initSocket(); });
    std::thread thread2([&] { ClientTCP::run(sample.c_str(), sw.getPort()); });
    thread1.join();
    thread2.join();
    Source src;
    src.openSocket(sw.getSocket());
    Source::NextCharacter letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 't');
    EXPECT_EQ(letter.absolutePosition, 1);
    EXPECT_EQ(letter.characterPosition, 1);
    EXPECT_EQ(letter.linePosition, 0);
    letter = src.getChar();
    EXPECT_EQ(letter.nextLetter, 'e');
    EXPECT_EQ(letter.absolutePosition, 2);
    EXPECT_EQ(letter.characterPosition, 2);
    EXPECT_EQ(letter.linePosition, 0);
}