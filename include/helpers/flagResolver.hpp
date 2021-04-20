#pragma once
#include <string>
#include <memory>

struct FlagResolver
{
    enum class Options
    {
        File,
        Socket,
        String,
        Help,
        Null
    };
    Options resolveOption(std::string option)
    {
        if (option == "--help" || option == "--h")
            return Options::Help;
        else if (option == "--file" || option == "--f")
            return Options::File;
        else if (option == "--socket" || option == "--sc")
            return Options::Socket;
        else if (option == "--string" || option == "--s")
            return Options::String;
        return Options::Null;
    }
};

using FlagResolverUptr = std::unique_ptr<FlagResolver>;