#include "program.hpp"

std::mutex Program::singletonMutex;

Program &Program::getInstance()
{
    static Program program;
    return program;
}

void Program::start(int argc, char **argv)
{

    if (argc == 1)
    {
        startInterpreter();
    }
    else
    {
        parseFlags(argv);
    }
}


void Program::parseFlags(char **argv)
{
    try
    {
        switch (flagResolver->resolveOption(argv[1]))
        {
        case (FlagResolver::Options::File):
            startWFile(argv[2]);
            break;
        case (FlagResolver::Options::Socket):
            startWSocket();
            break;
        case (FlagResolver::Options::String):
            startWString(argv[2]);
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
        std::cout << ex.what() <<"\n";
        return;
    }
}

void Program::showHelp(){

    std::cout<<"*******************************************************************\n";
    std::cout<<"*                       AVAILABLE FLAGS:                          *\n";
    std::cout<<"*   --help/-h shows help                                          *\n";
    std::cout<<"*   --string/-s <source string> parse code from string            *\n";
    std::cout<<"*   --file/-f <path to source file> parse code from file          *\n";
    std::cout<<"*   --socket/-sc  parse code from socket                          *\n";
    std::cout<<"*******************************************************************\n";
}

void Program::startInterpreter()
{
    std::cout << "Hello from interpreter!" << std::endl;
}


void Program::startWFile(std::string pathToFile){

    if(!std::filesystem::exists(pathToFile))
        throw CustomExceptions::Exception(CustomExceptions::ExceptionType::StringNotAPath,
                                              "String you provided is not a valid path!");
    source->openFile(pathToFile);
}


void Program::startWSocket(){
    source->openSocket();
}

void Program::startWString(std::string sourceString){
    source->openString(sourceString);
}