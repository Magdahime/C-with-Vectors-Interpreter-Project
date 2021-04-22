#pragma once
#include <fstream>
#include <string>
#include <unistd.h>

struct VisitSource
{
    void operator()(int &socket, uint64_t position, char *letter)
    {
        read(socket, letter, 1);
    }
    void operator()(std::fstream &sourceFile, uint64_t position, char *letter)
    {
        *letter = sourceFile.get();
    }
    void operator()(std::string &sourceString, uint64_t position, char *letter)
    {
        *letter = (sourceString[position]);
    }
};

struct ClosingVisitSource
{
    void operator()(int &socket)
    {
        close(socket);
    }

    void operator()(std::fstream &sourceFile)
    {
        sourceFile.close();
    }

    void operator()(std::string &sourceString)
    {
    }
};