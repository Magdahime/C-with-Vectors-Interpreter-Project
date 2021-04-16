#include "include/program.hpp"

std::mutex Program::singletonMutex;

Program& Program::getInstance()
{
    std::lock_guard<std::mutex> guard(singletonMutex);
    static Program program;
    
    return program;
}

void Program::start(){
    std::cout<<"Buongiorno Signora!"<<std::endl;
}