#pragma once
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <memory>

#include "exception.hpp"

class SocketWrapper {
 public:
  void initSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) throw SocketProblemException("Cannot open socket!");
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof server) == -1)
      throw SocketProblemException("Cannot bind socket!");
    listen(serverSocket, 5);
    int messageSocket = accept(serverSocket, nullptr, nullptr);
    if (messageSocket == -1)
      throw SocketProblemException("Cannot accept incoming connection!");
    else
      receiveSocket = messageSocket;
    close(serverSocket);
  }

  void deinitSocket() { close(receiveSocket); }

  uint getPort() const { return PORT; }
  uint getSocket() const { return receiveSocket; }

 private:
  const uint PORT = 35555;
  uint receiveSocket;
};

using SocketWrapperUptr = std::unique_ptr<SocketWrapper>;