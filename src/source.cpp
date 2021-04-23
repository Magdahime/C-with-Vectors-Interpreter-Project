#include "source.hpp"

Source::Source()
{
    positioner = std::make_unique<Positioner>();
    socketWrapper = std::make_unique<SocketWrapper>();
}
void Source::openFile(std::string filepath)
{
    codeSource = std::fstream();
    std::get<std::fstream>(codeSource).open(filepath, std::ios::out | std::ios::in);
}
void Source::openString(std::string sourceString)
{
    codeSource = sourceString;
}
void Source::openSocket(int socket)
{
    codeSource = socket;
}

Source::NextCharacter Source::getChar()
{
    char letter;
    std::visit(VisitSource(), codeSource,
               std::variant<uint64_t>(positioner->getAbsolutePosition()),
               std::variant<char *>(&letter));
    positioner->nextChar();
    if (letter == '\n')
        positioner->nextLine();
    return Source::NextCharacter(letter, positioner->getAbsolutePosition(),
                                positioner->getChar(), positioner->getLine());
}

Source::~Source()
{
    std::visit(ClosingVisitSource(), codeSource);
}