#include "program.hpp"

std::mutex Program::singletonMutex;

Program::Program()
{
    source = std::make_unique<Source>();
    flagResolver = std::make_unique<FlagResolver>();
};

Program &Program::getInstance()
{
    static Program program;
    return program;
}

void Program::start(int argc, std::vector<std::string> &arguments)
{

    if (argc == 1)
    {
        startInterpreter();
    }
    else
    {
        parseFlags(arguments);
    }
}

void Program::parseFlags(std::vector<std::string> &arguments)
{
    try
    {
        std::string sourceString = std::string(arguments[2]);
        switch (flagResolver->resolveOption(arguments[1]))
        {
        case (FlagResolver::Options::File):
            startWFile(arguments[2]);
            break;
        case (FlagResolver::Options::Socket):
            startWSocket(std::stoi(arguments[2]));
            break;
        case (FlagResolver::Options::String):
            startWString(sourceString);
            break;
        case (FlagResolver::Options::Help):
            showHelp();
            break;
        default:
            throw CustomExceptions::Exception(CustomExceptions::ExceptionType::InvalidFlagOptions,
                                              "Flags you provided are invalid. Try --help for help");
        }
    }
    catch (CustomExceptions::Exception &ex)
    {
        std::cout << ex.what() << "\n";
        return;
    }
}

void Program::showHelp()
{

    std::cout << "*******************************************************************\n";
    std::cout << "*                       AVAILABLE FLAGS:                          *\n";
    std::cout << "*   --help/-h shows help                                          *\n";
    std::cout << "*   --string/-s <source string> parse code from string            *\n";
    std::cout << "*   --file/-f <path to source file> parse code from file          *\n";
    std::cout << "*   --socket/-sc  <socket> parse code from socket                 *\n";
    std::cout << "*******************************************************************\n";
}

void Program::startInterpreter()
{
    std::cout << "Hello from interpreter!" << std::endl;
}

void Program::startWFile(std::string &pathToFile)
{

    if (!std::filesystem::exists(pathToFile))
        throw CustomExceptions::Exception(CustomExceptions::ExceptionType::StringNotAPath,
                                          "String you provided is not a valid path!");
    source->openFile(pathToFile);
}

void Program::startWSocket(int socket)
{
    source->openSocket(socket);
}

void Program::startWString(std::string &sourceString)
{
    source->openString(sourceString);
}