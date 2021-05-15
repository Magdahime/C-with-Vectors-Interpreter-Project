#pragma once
#include <filesystem>
#include <thread>
#include <vector>

#include "flagResolver.hpp"
#include "source.hpp"

namespace SourceFactory {
SourceUptr createSource(const FlagResolver::Options option,
                        const std::vector<std::string_view>& arguments);
}