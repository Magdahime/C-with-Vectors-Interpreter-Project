#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class Source{

public:

void open(std::string sourceString);
void open(int domain, int type, int protocol);
char getChar();
private:

};

using SourceUptr = std::unique_ptr<Source>;