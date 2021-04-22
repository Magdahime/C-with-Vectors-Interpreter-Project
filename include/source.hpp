#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <variant>
#include "helpers/positioner.hpp"
#include "helpers/socketWrapper.hpp"
#include "helpers/sourceVisitator.hpp"

class Source
{

public:
    Source();
    void openFile(std::string filepath);
    void openString(std::string sourceString);
    void openSocket();
    char getChar();
    ~Source();
    std::variant<std::string, int, std::fstream> codeSource;

private:
    PositionerUptr positioner;
    SocketWrapperUptr socketWrapper;
};

using SourceUptr = std::unique_ptr<Source>;