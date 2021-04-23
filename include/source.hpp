#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <memory>
#include <variant>
#include "helpers/positioner.hpp"
#include "helpers/socketWrapper.hpp"
#include "helpers/sourceVisitator.hpp"

class Source
{
public:
    struct NextCharacter
    {
        NextCharacter(char letter,uint64_t aPos, uint64_t cPos, uint64_t lPos) : 
                        nextLetter(letter), absolutePosition(aPos),
                        characterPosition(cPos), linePosition(lPos){}
        char nextLetter;
        uint64_t absolutePosition;
        uint64_t characterPosition;
        uint64_t linePosition;
    };

    Source();
    void openFile(std::string filepath);
    void openString(std::string sourceString);
    void openSocket(int socket);
    NextCharacter getChar();
    ~Source();
    std::variant<std::string, int, std::fstream> codeSource;

private:
    PositionerUptr positioner;
    SocketWrapperUptr socketWrapper;
};

using SourceUptr = std::unique_ptr<Source>;