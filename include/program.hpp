#pragma once
#include <filesystem>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "helpers/flagResolver.hpp"
#include "helpers/sourceFactory.hpp"
#include "lexicalAnalyzer/lexicalAnalyzer.hpp"

namespace Program {
extern SourceSptr source;
extern LexicalAnalyzerUptr lexicalAnalyzer;
void start(const int argc, const std::vector<std::string_view>& arguments);
void startInterpreter();
void parseFlags(const std::vector<std::string_view>& arguments);
void showHelp();
}  // namespace Program