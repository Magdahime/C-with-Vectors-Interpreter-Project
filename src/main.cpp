#include "program.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments;
    for (int i = 0; i < argc; ++i)
    {
        arguments.push_back(argv[i]);
    }
    
    Program::getInstance().start(argc, arguments);
    return 0;
}