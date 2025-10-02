#pragma once

#include <format>

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
    return ::lc_libs::TreeNodeBase<T>::print_dfs(ctx, view.node);
  }
};

}  // namespace std