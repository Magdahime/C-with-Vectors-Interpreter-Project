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
#include "interpreter/evaluator.hpp"
#include "lexicalAnalyzer/lexicalAnalyzer.hpp"
#include "parser/parser.hpp"

namespace Program {
extern SourceSptr source;
extern LexicalAnalyzerUptr lexicalAnalyzer;
extern ParserUptr parser;
extern EvaluatorUptr evaluator;
void start(const int argc, const std::vector<std::string_view>& arguments);
void startInterpreter();
void printVariablesAndFunctions(VariableMap varMap, FunctionMap funMap);
void parseFlags(const std::vector<std::string_view>& arguments);
void showHelp();

struct make_string_functor {
  std::string operator()(const std::string& x) const { return x; };
  std::string operator()(int64_t x) const { return std::to_string(x); }
  std::string operator()(double x) const { return std::to_string(x); }
  std::string operator()(Matrix x) const { return x.toString(); };
};

}  // namespace Program