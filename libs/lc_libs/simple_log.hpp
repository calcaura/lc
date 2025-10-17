#pragma once

#include <chrono>
#include <cstdlib>  // for std::getenv
#include <format>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <thirdParty/rang.hpp>
#include <utility>

#ifdef SANE
#define DIE_IF(cond, fmt, ...)                              \
  ({                                                        \
    if (cond) {                                             \
      std::cerr << __FILE__ << ": " << __LINE__ << " "      \
                << std::format(fmt, ##__VA_ARGS__) << "\n"; \
      exit(1);                                              \
    }                                                       \
  })
#else
#define DIE_IF(...) \
  do {              \
  } while (0)
#endif

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
      return "non";
    case L::Debug:
      return "dbg";
    case L::Info:
      return "inf";
    case L::Warn:
      return "wrn";
    case L::Error:
      return "err";
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
 public:
  using TimeProvider =
      std::function<std::chrono::time_point<std::chrono::system_clock>()>;

 private:
  friend class LogLevelGuard;
  friend class OutputGuard;
  friend class LogIndentGuard;
  friend class TimeProviderGuard;

  struct Internal {
    LogLevel level;
    std::string prefix;
    std::ostream* out;
    TimeProvider time_func;
  };

  static inline Internal internal{
      from_env(), "", &std::cout, []() {
        return std::chrono::floor<std::chrono::milliseconds>(
            std::chrono::system_clock::now());
      }};

 public:
  static LogLevel level() { return Logger::internal.level; }
  static const std::string& prefix() { return Logger::internal.prefix; }
  static std::ostream& out() { return *(Logger::internal.out); }
  static inline std::chrono::time_point<std::chrono::system_clock> time() {
    return Logger::internal.time_func();
  }
};

/**
 * A guard that sets the log level for its scope and restores the previous level
 * on destruction. Usage:
 * {
 *   LogLevelGuard guard(LogLevel::Debug);
 *   // Log level is Debug in this scope
 * }
 * // Log level is restored to previous level
 */
class LogLevelGuard {
 public:
  LogLevelGuard(LogLevel new_level) : old_level_(Logger::level()) {
    Logger::internal.level = new_level;
  }
  ~LogLevelGuard() { Logger::internal.level = old_level_; }

 private:
  LogLevel old_level_;
};

/**
 * A guard that increases the log indentation for its scope and restores the
 * previous indentation on destruction. Usage:
 * {
 *   LogIndentGuard indent_guard;
 *   // Log messages are indented in this scope
 * }
 * // Indentation is restored to previous level
 */
class LogIndentGuard {
 public:
  LogIndentGuard() { Logger::internal.prefix += " "; }
  ~LogIndentGuard() { Logger::internal.prefix.pop_back(); }
};

/**
 * A guard that sets the log output stream for its scope and restores the
 * previous output stream on destruction. Usage:
 * {
 *   OutputGuard guard(new_output_stream);
 *   // Log messages are written to new_output_stream in this scope
 * }
 * // Output stream is restored to previous stream
 */
class OutputGuard {
 public:
  OutputGuard(std::ostream& new_out) : old_out_(Logger::out()) {
    Logger::internal.out = &new_out;
  }
  ~OutputGuard() { Logger::internal.out = &old_out_; }

 private:
  std::ostream& old_out_;
};

class TimeProviderGuard {
 public:
  TimeProviderGuard(Logger::TimeProvider new_time_func)
      : old_time_func_(Logger::internal.time_func) {
    Logger::internal.time_func = std::move(new_time_func);
  }
  ~TimeProviderGuard() { Logger::internal.time_func = old_time_func_; }

 private:
  Logger::TimeProvider old_time_func_;
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
  auto format(const lc_libs::log::LogLevel& l, FormatContext& ctx) const {
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

  Logger::out() << fg;
  auto time = Logger::time();

  {
    auto it = std::ostream_iterator<char>(Logger::out());
    it = std::format_to(it, "{:%FT%T} [{}] - {}", time, c_str(level),
                        Logger::prefix());
    it = std::vformat_to(it, fmt.get(), std::make_format_args(args...));
  }
  Logger::out() << rang::style::reset << std::endl;
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