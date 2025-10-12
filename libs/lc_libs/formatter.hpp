#pragma once

#include <format>

namespace lc_libs::formatter {

/**
 * A base formatter that supports alignment and width for custom types.
 * Inherit from this class and implement the static method `format_unaligned`
 * that formats the type without any alignment or width considerations.
 */
template <typename T, size_t TempBuffSize = 256>
struct aligned_formatter {
  using FormatterType = ::std::formatter<T>;

  char fill = ' ';
  char align = '\0';
  size_t width = 0;

  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    auto it = ctx.begin();
    auto end = ctx.end();

    if (it == end) return it;  // empty specifier

    // Try to parse fill + align
    if ((it + 1) != end &&
        (*(it + 1) == '<' || *(it + 1) == '>' || *(it + 1) == '^')) {
      fill = *it;
      align = *(it + 1);
      it += 2;
    } else if (*it == '<' || *it == '>' || *it == '^') {
      align = *it;
      ++it;
    }

    // Optionally parse width
    while (it != end && *it >= '0' && *it <= '9') {
      width = width * 10 + (*it - '0');
      ++it;
    }

    // Return iterator to end of this format spec
    return it;
  }

  // Formats the value into the context
  template <typename FormatContext>
  auto format(const T& s, FormatContext& ctx) const -> decltype(ctx.out()) {
    auto out = ctx.out();

    if (align == '\0' || align == '<' || width == 0) {
      auto result = FormatterType::format_unaligned(s, out, TempBuffSize);
      out = result.out;
      if (align == '\0' || width == 0 || result.size >= (long)TempBuffSize) {
        return out;
      }
    }

    // Format on the stack to avoid dynamic memory allocation
    std::array<char, TempBuffSize> temp_buff;
    auto result = FormatterType::format_unaligned(
        s, temp_buff.begin(),
        TempBuffSize - 1);  // reserve 1 for null-terminator
    if (result.size >= (long)TempBuffSize) {
      return std::format_to(out, "{}", temp_buff.data());
    }
    size_t distance = result.out - temp_buff.begin() + 1;
    temp_buff[std::min(distance, TempBuffSize - 1)] = '\0';

    auto fillb = [fill = this->fill](auto it, size_t count) {
      for (size_t i = 0; i < count; ++i) {
        *it++ = fill;
      }
      return it;
    };

    if (align == '<') {
      if (width > 0 && distance < width) {
        return fillb(out, width - distance);
      }
      return out;
    }

    if (align == '>') {
      if (distance < width) {
        out = fillb(out, width - distance);
      }
      return std::format_to(out, "{}", temp_buff.data());
    }

    const int total_width = width;
    const int content_width = distance;
    const int padding =
        total_width > content_width ? total_width - content_width : 0;
    const int left_padding = padding / 2;
    const int right_padding = padding - left_padding;

    out = fillb(out, left_padding);
    out = std::format_to(out, "{}", temp_buff.data());
    return fillb(out, right_padding);
  };
};
}  // namespace lc_libs::formatter