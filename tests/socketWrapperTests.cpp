#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "helpers/socketWrapper.hpp"
const char message[1024] = "Hello world!";

void client()
{
  try
  {
    struct sockaddr_in server;
    int sendSocket;
    SocketWrapper sw;

    sendSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sendSocket == -1)
      throw "Error cannot open socket!";

    struct hostent *hp;

    server.sin_family = AF_INET;
    server.sin_port = htons(sw.getPort());

    hp = gethostbyname("localhost");

    if (hp == (struct hostent *)0)
      throw "Error cannot find host!";

    memcpy((char *)&server.sin_addr, (char *)hp->h_addr,
           hp->h_length);

    {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1000ms);
    }

    if (connect(sendSocket, (struct sockaddr *)&server, sizeof server) == -1)
    {
      throw "Error in connecting to server";
    }
    if (write(sendSocket, message, strlen(message)) == -1)
      throw "Error in sending data!";
    close(sendSocket);
  }
  catch (const char *msg)
  {
    std::cerr << msg << std::endl;
    exit(1);
  }
}

TEST(SocketWrapperTest, IsSendingDataWorks)
{
  SocketWrapper sw;
  std::thread thread1([&] { sw.initSocket(); });
  std::thread thread2([&] { client(); });
  thread1.join();
  thread2.join();

  char messageSent[1024];
  memset(messageSent, 0, sizeof messageSent);
  int messageSize = read(sw.getSocket(), messageSent, 1024);
  EXPECT_EQ(messageSize, strlen(message));
  EXPECT_EQ(strcmp(messageSent, message), 0);
}
