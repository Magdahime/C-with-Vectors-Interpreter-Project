#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>

struct ClientTCP
{
  static void run(const char *message, int PORT)
  {
    try
    {
      struct sockaddr_in server;
      int sendSocket;

      sendSocket = socket(AF_INET, SOCK_STREAM, 0);
      if (sendSocket == -1)
        throw "Error cannot open socket!";

      struct hostent *hp;

      server.sin_family = AF_INET;
      server.sin_port = htons(PORT);

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
};