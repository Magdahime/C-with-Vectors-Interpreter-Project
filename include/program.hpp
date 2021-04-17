#pragma once
#include <iostream>
#include <mutex>
#include <filesystem>
#include "exception.hpp"
#include "source.hpp"

class Program
{
public:
    void start(int argc, char **argv);
    static Program &getInstance();

private:
    SourceUptr source;
    enum class Options{
        File,
        Socket,
        String,
        Help,
        Null
    };
    static std::mutex singletonMutex;
    void startInterpreter();
    void parseFlags(char **argv);
    void startWFile(std::string pathToFile);
    void startWString(std::string sourceString);
    void startWSocket(char **argv);
    void showHelp();
    Options resolveOption(std::string option);

};