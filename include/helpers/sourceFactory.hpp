#pragma once
#include <vector>
#include <filesystem>
#include <thread>
#include "source.hpp"
#include "flagResolver.hpp"

namespace SourceFactory{
    SourceUptr createSource(const FlagResolver::Options option, const std::vector<std::string_view>& arguments);
}