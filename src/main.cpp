#include "program.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string_view> arguments;
    for (int i = 0; i < argc; ++i)
    {
        arguments.push_back(argv[i]);
    }

    Program::start(argc, arguments);
    return 0;
}