#pragma once
#include <iostream>
#include <mutex>
#include <filesystem>
#include "helpers/exception.hpp"
#include "source.hpp"
#include "helpers/flagResolver.hpp"

class Program
{
public:
    void start(int argc, char **argv);
    static Program &getInstance();

private:
    Program() = default;
    ~Program() = default;
    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
    SourceUptr source;
    FlagResolverUptr flagResolver;

    static std::mutex singletonMutex;
    void startInterpreter();
    void startWFile(std::string pathToFile);
    void startWString(std::string sourceString);
    void startWSocket();
    void parseFlags(char **argv);
    void showHelp();
};
