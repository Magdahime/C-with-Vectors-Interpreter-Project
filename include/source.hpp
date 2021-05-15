#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "helpers/position.hpp"
#include "helpers/socketWrapper.hpp"

struct NextCharacter {
  NextCharacter() = default;
  NextCharacter(char letter, uint64_t aPos, uint64_t cPos, uint64_t lPos)
      : nextLetter(letter),
        absolutePosition(aPos),
        characterPosition(cPos),
        linePosition(lPos) {}
  std::string getLinePosition() const {
    return std::to_string(characterPosition) + ":" +
           std::to_string(linePosition);
  }

  char nextLetter;
  uint64_t absolutePosition;
  uint64_t characterPosition;
  uint64_t linePosition;
};

class SourceBase {
 public:
  virtual void open() = 0;
  virtual NextCharacter getChar() = 0;
  virtual void close() = 0;
  NextCharacter getCurrentCharacter() const { return currentCharacter; }
  virtual ~SourceBase() = default;

 protected:
  NextCharacter currentCharacter;
};

using SourceUptr = std::unique_ptr<SourceBase>;
using SourceSptr = std::shared_ptr<SourceBase>;

class FileSource : public SourceBase {
 public:
  void open() override;
  void close() override;
  NextCharacter getChar() override;
  FileSource(const std::string_view filepath)
      : filepath(std::filesystem::path(filepath)), position(Position()) {}
  ~FileSource() { close(); }

 private:
  std::filesystem::path filepath;
  std::fstream fileSource;
  Position position;
};

class SocketSource : public SourceBase {
 public:
  void open() override;
  void close() override;
  NextCharacter getChar() override;
  SocketSource() : position(Position()) { socketWrapper.initSocket(); }
  void waitForData();
  int getPort() const { return socketWrapper.getPort(); }
  ~SocketSource() { close(); }

 private:
  uint socketSource;
  SocketWrapper socketWrapper;
  Position position;
};

class StringSource : public SourceBase {
 public:
  void open() override;
  void close() override {}
  NextCharacter getChar() override;
  StringSource(const std::string_view codeSource)
      : sourceIterator(codeSource.begin()),
        stringSource(codeSource),
        position(Position()) {}
  ~StringSource() { close(); }

 private:
  std::string_view::iterator sourceIterator;
  std::string_view stringSource;
  Position position;
};