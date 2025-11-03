add_library(
db STATIC
optimized_group_by.cpp
reference_group_by.cpp
)

target_include_directories(
  db PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
  db
  absl::base
  absl::random_random
  benchmark::benchmark
)
