add_executable(group_by_bench group_by_bench.cpp)

target_include_directories(
  group_by_bench PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
  group_by_bench PUBLIC
  db
  absl::base
  absl::random_random
  benchmark::benchmark
)
