#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <variant>
#include "helpers/positioner.hpp"

class Source{

public:

void openFile(std::string filepath);
void openString(std::string sourceString);
void openSocket(int domain, int type, int protocol);
char getChar();
~Source();

private:
PositionerUptr positioner;
std::variant<int, std::fstream, std::string> source;

};

using SourceUptr = std::unique_ptr<Source>;