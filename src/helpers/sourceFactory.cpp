#include "helpers/sourceFactory.hpp"
SourceUptr SourceFactory::createSource(FlagResolver::Options option, 
                                       const std::vector<std::string_view> &arguments)
{
    switch(option){
        case(FlagResolver::Options::File):
            return std::make_unique<FileSource>(createFileSource(arguments[2]));
        case(FlagResolver::Options::Socket):
            return std::make_unique<SocketSource>(createSocketSource());
        case(FlagResolver::Options::String):
            return std::make_unique<StringSource>(createStringSource(arguments[2]));
        default:
            throw CannotCreateSourceException("Wrong options!");
    }
}

FileSource SourceFactory::createFileSource(const std::string_view& filepath){
    return FileSource(filepath);
}
SocketSource SourceFactory::createSocketSource(){
    SocketSource src = SocketSource();
    std::cout<<"Please wait for data to TCP socket to arrive..."<<std::endl;
    src.waitForData();
    return src;
}
StringSource SourceFactory::createStringSource(const std::string_view& codeSource){
    return StringSource(codeSource);
}