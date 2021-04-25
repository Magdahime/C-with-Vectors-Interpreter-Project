#pragma once
#include <vector>
#include <filesystem>
#include <thread>
#include "source.hpp"
#include "flagResolver.hpp"

class SourceFactory{

public:
    static SourceFactory& getInstance();
    SourceUptr createSource(const FlagResolver::Options option, const std::vector<std::string_view>& arguments);
private:
    FileSource createFileSource(const std::string_view& filepath);
    SocketSource createSocketSource();
    StringSource createStringSource(const std::string_view& codeSource);
};