#pragma once

#include <format>
#include <iostream>

#include "tree_node.hpp"

// -------------------- std::formatter specializations --------------------
// Must be real specializations in namespace std so std::format recognizes them.

namespace std {

template <typename T, typename CharT>
struct formatter<::lc_libs::DfsView<T>, CharT> {
  // return type for parse must match the parse context iterator type
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  // format must be a template accepting the library's FormatContext and return
  // the iterator (decltype(ctx.out())).
  template <typename FormatContext>
  auto format(const ::lc_libs::DfsView<T>& view, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    bool comma{};
    auto result = ctx.out();
    for (auto it : view) {
      if (comma) {
        result = std::format_to(result, ", ");
      }
      comma = true;
      result = std::format_to(result, "{}", it);
    }
    return result;
  }
};

}  // namespace std

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const typename ::lc_libs::DfsView<T>& view) {
  bool comma{};
  for (auto it : view) {
    if (comma) {
      os << ", ";
    }
    comma = true;
    os << it;
  }
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const typename ::lc_libs::BfsView<T>& view) {
  bool comma{};
  for (auto it : view) {
    if (comma) {
      os << ", ";
    }
    comma = true;
    os << it;
  }
  return os;
}