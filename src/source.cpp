#include "source.hpp"

void FileSource::open() {
  fileSource.open(filepath, std::fstream::in);
  currentCharacter = getChar();
}
void SocketSource::open() {
  socketSource = socketWrapper.getSocket();
  currentCharacter = getChar();
}

void StringSource::open() { currentCharacter = getChar(); }

void FileSource::close() { fileSource.close(); }
void SocketSource::close() { ::close(socketSource); }

NextCharacter StringSource::getChar() {
  char letter = *sourceIterator;
  currentCharacter = NextCharacter(letter, position.getAbsolutePosition(),
                                   position.getChar(), position.getLine());
  position = position.nextChar();
  sourceIterator++;
  if (letter == '\n') {
    position = position.nextLine();
  }
  return currentCharacter;
}

NextCharacter FileSource::getChar() {
  char letter = fileSource.get();
  if (fileSource.eof()) letter = '\0';
  currentCharacter = NextCharacter(letter, position.getAbsolutePosition(),
                                   position.getChar(), position.getLine());
  position = position.nextChar();
  if (letter == '\n') {
    position = position.nextLine();
  }
  return currentCharacter;
}

NextCharacter SocketSource::getChar() {
  char letter[1];
  read(socketSource, letter, 1);
  currentCharacter = NextCharacter(*(letter), position.getAbsolutePosition(),
                                   position.getChar(), position.getLine());
  position = position.nextChar();
  if (letter[0] == '\n') {
    position = position.nextLine();
  }
  return currentCharacter;
}
