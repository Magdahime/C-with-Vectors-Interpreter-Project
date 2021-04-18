#pragma once
#include <memory>
class Positioner{
public:
    void nextChar();
    void nextLine();
    uint64_t getChar(){return character;}
    uint64_t getLine(){return line;}
    uint64_t getAbsolutePosition(){return absolutePosition;}
private:
    uint64_t absolutePosition;
    uint64_t line;
    uint64_t character;
};

using PositionerUptr = std::unique_ptr<Positioner>;