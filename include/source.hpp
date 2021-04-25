#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <memory>
#include "helpers/positioner.hpp"
#include "helpers/socketWrapper.hpp"

struct NextCharacter
{
    NextCharacter() = default;
    NextCharacter(char letter, uint64_t aPos, uint64_t cPos, uint64_t lPos) : nextLetter(letter), absolutePosition(aPos),
                                                                              characterPosition(cPos), linePosition(lPos) {}
    char nextLetter;
    uint64_t absolutePosition;
    uint64_t characterPosition;
    uint64_t linePosition;
};

class SourceBase
{
public:
    virtual void open() = 0;
    virtual NextCharacter getChar() = 0;
    virtual void close() = 0;
    NextCharacter getCurrentCharacter() { return currentCharacter; }

protected:
    NextCharacter currentCharacter;
    SourceBase() = default;
};

using SourceUptr = std::unique_ptr<SourceBase>;
using SourceSptr = std::shared_ptr<SourceBase>;

class FileSource : public SourceBase
{
public:
    virtual void open();
    virtual void close();
    virtual NextCharacter getChar() override;
    FileSource(const std::string_view &filepath)
    {
        this->filepath = std::filesystem::path(filepath);
        positioner = std::make_unique<Positioner>(Positioner());
    }

private:
    std::filesystem::path filepath;
    std::fstream fileSource;
    PositionerUptr positioner;
};

class SocketSource : public SourceBase
{
public:
    virtual void open();
    virtual void close();
    virtual NextCharacter getChar() override;
    SocketSource()
    {
        socketWrapper = std::make_unique<SocketWrapper>(SocketWrapper());
        positioner = std::make_unique<Positioner>(Positioner());
    }
    void waitForData();
    int getPort()const {return socketWrapper->getPort();}
private:
    int socketSource;
    SocketWrapperUptr socketWrapper;
    PositionerUptr positioner;
};

class StringSource : public SourceBase
{
public:
    virtual void open() {}
    virtual void close() {}
    virtual NextCharacter getChar() override;
    StringSource(const std::string_view &codeSource) : stringSource(codeSource)
    {
        positioner = std::make_unique<Positioner>(Positioner());
    }

private:
    std::string_view stringSource;
    PositionerUptr positioner;
};