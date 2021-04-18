#include "source.hpp"

void Source::openFile(std::string filepath)
{
    source = std::fstream();
    std::get<std::fstream>(source).open(filepath, std::ios::out);
}
void Source::openString(std::string sourceString)
{
    source = sourceString;
}
void Source::openSocket(int domain, int type, int protocol)
{
}

char Source::getChar()
{
    return 'a';
}

Source::~Source(){
    
}