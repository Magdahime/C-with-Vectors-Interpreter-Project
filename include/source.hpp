#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Source{

public:

void open(std::string sourceString);
void open(int domain, int type, int protocol);
char getChar();

private:

};