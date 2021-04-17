#include "program.hpp"


int main(int argc, char *argv[]){
    Program::getInstance().start(argc, argv);
    return 0;
}