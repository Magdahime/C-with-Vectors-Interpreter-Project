#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <memory>
#include "exception.hpp"

class SocketWrapper
{
public:
    void initSocket()
    {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
            throw CustomExceptions::Exception(CustomExceptions::ExceptionType::SocketProblem,
                                              "Cannot open socket!");
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);
        if (bind(serverSocket, (struct sockaddr *)&server, sizeof server) == -1)
            throw CustomExceptions::Exception(CustomExceptions::ExceptionType::SocketProblem,
                                              "Cannot bind socket!");
        listen(serverSocket, 5);
        int messageSocket = accept(serverSocket, nullptr, nullptr);
        if (messageSocket == -1)
            throw CustomExceptions::Exception(CustomExceptions::ExceptionType::SocketProblem,
                                              "Cannot accept incoming connection!");
        else
            receiveSocket = messageSocket;
        close(serverSocket);
    }

    void deinitSocket()
    {

        close(receiveSocket);
    }
    int getPort() { return PORT; }
    int getSocket() {return receiveSocket;}

private:
    const int PORT = 35555;
    int receiveSocket;
};

using SocketWrapperUptr = std::unique_ptr<SocketWrapper>;