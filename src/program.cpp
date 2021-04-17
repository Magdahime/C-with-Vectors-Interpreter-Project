#include "program.hpp"

std::mutex Program::singletonMutex;

Program &Program::getInstance()
{
    std::lock_guard<std::mutex> guard(singletonMutex);
    static Program program;

    return program;
}

void Program::start(int argc, char **argv)
{
    std::cout << "Buongiorno Signora!" << std::endl;

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
        switch (resolveOption(argv[1]))
        {
        case (Options::File):
            startWFile(argv[2]);
            break;
        case (Options::Socket):
            startWSocket(argv);
            break;
        case (Options::String):
            startWString(argv[2]);
            break;
        case (Options::Help):
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
    std::cout<<"*   --help shows help                                             *\n";
    std::cout<<"*   --string <source string> parse code from string               *\n";
    std::cout<<"*   --file <path to source file> parse code from file             *\n";
    std::cout<<"*   --socket <source string> parse code from string               *\n";
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
    
}

Program::Options Program::resolveOption(std::string option){

    if(option == "--help" || option == "--h"){
        return Options::Help;
    }else if(option == "--file" || option == "--f"){
        return Options::File;
    }else if(option == "--socket" || option == "--sc"){
        return Options::Socket;
    }else if(option == "--string" || option == "--s"){
        return Options::String;
    }
    return Options::Null;
}

void Program::startWSocket(char **argv){

}

void Program::startWString(std::string sourceString){

}