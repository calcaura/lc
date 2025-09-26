#pragma once

#include <chrono>
#include <cstdlib>  // for std::getenv
#include <format>
#include <iostream>
#include <thirdParty/rang.hpp>
#include <utility>

namespace lc_libs::log {

enum class LogLevel : uint8_t {
  None = 0,
  Error = 1,
  Warn = 2,
  Info = 3,
  Debug = 4
};

static inline const char* c_str(const lc_libs::log::LogLevel p) {
  using L = lc_libs::log::LogLevel;
  switch (p) {
    case L::None:
      return "none";
    case L::Debug:
      return "debug";
    case L::Info:
      return "info";
    case L::Warn:
      return "warn";
    case L::Error:
      return "error";
  }
  return "unknown";
}

static inline LogLevel from(const std::string_view s) {
  if (s == "none") {
    return LogLevel::None;
  } else if (s == "debug") {
    return LogLevel::Debug;
  } else if (s == "info") {
    return LogLevel::Info;
  } else if (s == "warn") {
    return LogLevel::Warn;
  } else if (s == "error") {
    return LogLevel::Error;
  }
  return LogLevel::None;
}

static inline LogLevel from_env(LogLevel default_level = LogLevel::Info) {
  auto env = std::getenv("LOG_LEVEL");
  if (env) {
    return from(env);
  }
  return default_level;
}

class Logger {
 private:
  struct Internal {
    LogLevel level;
  };

  static inline Internal internal{from_env()};

 public:
  static LogLevel level() { return Logger::internal.level; }
};

}  // namespace lc_libs::log

namespace std {
template <>
struct std::formatter<lc_libs::log::LogLevel> : std::formatter<std::string> {
  // Parses the format specifier (optional)
  constexpr auto parse(std::format_parse_context& ctx) {
    // You can parse things like ":hex" here if you want custom options.
    return ctx.begin();
  }

  // Formats the value into the context
  template <typename FormatContext>
  auto format(const lc_libs::log::LogLevel& l, FormatContext& ctx) {
    std::format_to(ctx.out(), c_str(l));
  };
};

}  // namespace std

namespace lc_libs::log {

template <typename... Args>
void _colored(rang::fg fg, LogLevel level, std::format_string<Args...> fmt,
              Args&&... args) {
  auto crt_level = Logger::level();
  if (crt_level < level) {
    return;
  }

  std::cout << fg;
  auto time = std::chrono::floor<std::chrono::milliseconds>(
      std::chrono::system_clock::now());

  {
    auto it = std::ostream_iterator<char>(std::cout);
    it = std::format_to(it, "{:%FT%T} [{}] - ", time, c_str(level));
    it = std::vformat_to(it, fmt.get(), std::make_format_args(args...));
  }
  std::cout << rang::style::reset << std::endl;
}

template <typename... Args>
void debug(std::format_string<Args...> fmt, Args&&... args) {
  _colored(rang::fg::blue, LogLevel::Debug, std::forward<decltype(fmt)>(fmt),
           std::forward<Args>(args)...);
}
template <typename... Args>
void info(std::format_string<Args...> fmt, Args&&... args) {
  _colored(rang::fg::green, LogLevel::Info, std::forward<decltype(fmt)>(fmt),
           std::forward<Args>(args)...);
}
template <typename... Args>
void warn(std::format_string<Args...> fmt, Args&&... args) {
  _colored(rang::fg::yellow, LogLevel::Warn, std::forward<decltype(fmt)>(fmt),
           std::forward<Args>(args)...);
}

template <typename... Args>
void error(std::format_string<Args...> fmt, Args&&... args) {
  _colored(rang::fg::red, LogLevel::Error, std::forward<decltype(fmt)>(fmt),
           std::forward<Args>(args)...);
}
}  // namespace lc_libs::log