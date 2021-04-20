#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <variant>
#include "helpers/positioner.hpp"
#include "helpers/socketWrapper.hpp"

class Source{

public:

void openFile(std::string filepath);
void openString(std::string sourceString);
void openSocket();
char getChar();
~Source();

private:
PositionerUptr positioner;
SocketWrapperUptr socketWrapper;
std::variant<int, std::fstream, std::string> source;

};

using SourceUptr = std::unique_ptr<Source>;