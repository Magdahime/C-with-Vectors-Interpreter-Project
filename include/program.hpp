#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <string_view>
#include <mutex>
#include <filesystem>
#include "helpers/sourceFactory.hpp"
#include "helpers/flagResolver.hpp"
#include "lexical_analyzer/lexicalAnalyzer.hpp"

class Program
{
public:
    void start(const int argc, const std::vector<std::string_view>& arguments);
    static Program &getInstance();

private:
    Program();
    ~Program() = default;
    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
    SourceSptr source;
    FlagResolverUptr flagResolver;
    LexicalAnalyzerUptr lexicalAnalyzer;

    void startInterpreter() const;
    void openSource();
    void parseFlags(const std::vector<std::string_view>& arguments);
    void showHelp() const;
};
