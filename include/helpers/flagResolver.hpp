#pragma once
#include <string>

namespace FlagResolver {
enum class Options { File, Socket, String, Help, Null };
constexpr Options resolveOption(const std::string_view option) {
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
}  // namespace FlagResolver