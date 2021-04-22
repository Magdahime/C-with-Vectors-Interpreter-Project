#include <gtest/gtest.h>
#include "source.hpp"
#include <thread>
#include "clientTCP.hpp"

std::string sample = "test";

TEST(SourceTest, openStringTest)
{
    Source src;
    src.openString(sample);
    char letter = src.getChar();
    EXPECT_EQ(letter, 't');
    letter = src.getChar();
    EXPECT_EQ(letter, 'e');

}

TEST(SourceTest, openFileTest)
{
    Source src;
    src.openFile("../tests/res/sampleText.txt");
    char letter = src.getChar();
    EXPECT_EQ(letter, 'L');
    letter = src.getChar();
    EXPECT_EQ(letter, 'o');
}

TEST(SourceTest, openSocketTest){
    SocketWrapper sw;
    std::thread thread1([&] { sw.initSocket(); });
    std::thread thread2([&] { ClientTCP::run(sample.c_str(), sw.getPort()); });
    thread1.join();
    thread2.join();
    Source src;
    src.openSocket(sw.getSocket());
    char letter = src.getChar();
    EXPECT_EQ(letter, 't');
    letter = src.getChar();
    EXPECT_EQ(letter, 'e');
}