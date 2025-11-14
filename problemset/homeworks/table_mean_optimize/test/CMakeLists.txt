# Setup Google Test
enable_testing()
include(GoogleTest)

add_executable(
  tests
  tests.cpp
)

target_include_directories(
  tests PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
  tests PUBLIC
  db
  GTest::gtest_main
)

gtest_discover_tests(tests)
