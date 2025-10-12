#pragma once

#include <string_view>

namespace lc_libs::strings {

/**
 * Splits the input string `s` by the given `delimiter` and returns a container
 * of type `R` (e.g., std::vector, std::list) containing the split parts.
 *
 * Example usage:
 *   auto parts = lc_libs::strings::split<std::vector<std::string>>("a,b,c",
 * ',');
 *   // parts will be {"a", "b", "c"}
 *
 * Template Parameters:
 *   R: The type of container to return (must support push_back).
 *   CharT: The character type of the input string (default is char).
 *
 * Parameters:
 *   s: The input string to split (can be std::string or std::string_view).
 *   delimiter: The character to split the string by.
 *
 * Returns:
 *   A container of type R containing the split parts of the string.
 */
template <typename R, typename CharT = char>
R split(std::basic_string_view<CharT> s, CharT delimiter) {
  size_t start = 0;
  size_t end = s.find(delimiter);
  R result;
  while (end != std::string_view::npos) {
    result.push_back(s.substr(start, end - start));
    start = end + 1;
    end = s.find(delimiter, start);
  }
  result.push_back(s.substr(start));
  return result;
}

/**
 * Overload of split function to accept std::string as input.
 */
template <typename R, typename CharT = char>
R split(std::basic_string<CharT> &s, CharT delimiter) {
  return split<R, CharT>(std::basic_string_view<CharT>(s), delimiter);
}

}  // namespace lc_libs::strings