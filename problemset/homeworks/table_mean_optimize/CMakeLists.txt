cmake_minimum_required(VERSION 3.17)

project(devtech_cpu_interview LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
  "$<$<CONFIG:RELEASE>:-march=native>"
  "$<$<CONFIG:RELEASE>:-mtune=native>"
  # "$<$<CONFIG:RELEASE>:-mcpu=native>"
  "-Wall"
  "-Wextra"
)

# needed for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "Enable Abseil C++ Standard flag propagation")
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable Google Benchmark tests")
set(BUILD_GMOCK OFF CACHE BOOL "Disable GMock")

find_package(absl REQUIRED)
find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(src)
