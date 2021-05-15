#pragma once
#include <optional>
namespace Operators {
template <class T>
std::optional<T> operator|(const std::optional<T> &lhs,
                           const std::optional<T> &rhs) {
  if (lhs) return lhs;
  return rhs;
}
}  // namespace Operators
