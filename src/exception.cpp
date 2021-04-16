#include "exception.hpp"

CustomExceptions::Exception::Exception(ExceptionType type, const std::string &message) 
                                       : type(type), message(message){}

CustomExceptions::Exception::Exception(ExceptionType type, const std::string &message,
                                       std::string function, int line, int position) : 
                                       type(type){
    if(!function.empty()){
        this->message = message + " in " + function + ", at: " 
                        + std::to_string(line) + ":" + std::to_string(position);
    }
    this->message = message + " at: " 
                    + std::to_string(line) + ":" + std::to_string(position);
}

const char* CustomExceptions::Exception::what() const{
    return message.c_str();
}