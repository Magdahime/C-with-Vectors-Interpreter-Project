#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <string_view>
#include <mutex>
#include <optional>
#include <filesystem>
#include "helpers/sourceFactory.hpp"
#include "helpers/flagResolver.hpp"
#include "lexical_analyzer/lexicalAnalyzer.hpp"

namespace Program
{
    extern SourceSptr source;
    extern LexicalAnalyzerUptr lexicalAnalyzer;
    void start(const int argc, const std::vector<std::string_view>& arguments);
    void startInterpreter();
    void parseFlags(const std::vector<std::string_view>& arguments);
    void showHelp();
}