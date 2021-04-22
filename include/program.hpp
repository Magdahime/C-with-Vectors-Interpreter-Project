#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include <filesystem>
#include "helpers/exception.hpp"
#include "source.hpp"
#include "helpers/flagResolver.hpp"

class Program
{
public:
    void start(int argc, std::vector<std::string>& arguments);
    static Program &getInstance();

private:
    Program();
    ~Program() = default;
    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
    SourceUptr source;
    FlagResolverUptr flagResolver;

    static std::mutex singletonMutex;
    void startInterpreter();
    void startWFile(std::string& pathToFile);
    void startWString(std::string& sourceString);
    void startWSocket(int socket);
    void parseFlags(std::vector<std::string>& arguments);
    void showHelp();
};
