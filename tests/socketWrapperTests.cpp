#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "helpers/socketWrapper.hpp"
#include "clientTCP.hpp"


const char message[1024] = "Hello world!";

TEST(SocketWrapperTest, IsSendingDataWorks)
{
  SocketWrapper sw;
  std::thread thread1([&] { sw.initSocket(); });
  std::thread thread2([&] { ClientTCP::run(message, sw.getPort()); });
  thread1.join();
  thread2.join();

  char messageSent[1024];
  memset(messageSent, 0, sizeof messageSent);
  int messageSize = read(sw.getSocket(), messageSent, 1024);
  EXPECT_EQ(messageSize, strlen(message));
  EXPECT_EQ(strcmp(messageSent, message), 0);
  sw.deinitSocket();
}
