#include "helpers/sourceFactory.hpp"
SourceUptr SourceFactory::createSource(
    FlagResolver::Options option,
    const std::vector<std::string_view> &arguments) {
  switch (option) {
    case (FlagResolver::Options::File):
      return std::make_unique<FileSource>(arguments[2]);
    case (FlagResolver::Options::Socket):
      return std::make_unique<SocketSource>();
    case (FlagResolver::Options::String):
      return std::make_unique<StringSource>(arguments[2]);
    default:
      throw CannotCreateSourceException("Wrong options!");
  }
}
