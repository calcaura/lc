#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <vector>

#include "max_sequence.hpp"

#ifdef INCLUDE_MAIN

/*
 * N: 0 < homes ≤ 10,000 (maximum number of homes on the block)
 * S: 0 ≤ max ≤ 1000 ( maximum number of pieces of candy that the child may
 * collect) A: 0 ≤ pieces ≤ 1000 (representing the number of pieces given at
 * each home)
 */

constexpr size_t kMaxHomes = 10000;    // N
constexpr uint32_t kMaxCandy = 1000;   // S
constexpr uint32_t kMaxPieces = 1000;  // A

// Reads the input from the given file
static std::tuple<std::vector<uint32_t>, size_t, std::string> read_input(
    const char* input_file_name) {
  std::ifstream input_file(input_file_name);
  if (!input_file) {
    return {
        {}, 0, std::format("Failed to open input file: {}", input_file_name)};
  }

  size_t out_homes = 0;
  uint32_t out_max_candy = 0;

  // Read number of homes
  input_file >> out_homes;
  if (out_homes == 0 || out_homes > kMaxHomes) {
    return {{}, 0, std::format("Invalid number of homes: {}", out_homes)};
  }

  input_file >> out_max_candy;
  if (out_max_candy > kMaxCandy) {
    return {{}, 0, std::format("Invalid maximum candy: {}", out_max_candy)};
  }

  std::vector<uint32_t> input(out_homes);
  for (size_t i = 0; i < out_homes; ++i) {
    input_file >> input[i];
    if (input[i] > kMaxPieces) {
      return {
          {},
          0,
          std::format("Invalid number of pieces at home {}: {}", i, input[i])};
    }
  }

  if (!input_file) {
    return {{}, 0, "Error reading input file (incorrect format?)"};
  }

  return {std::move(input), out_max_candy, ""};
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }
  const char* input_file_name = argv[1];
  auto [input, max_candy, error] = read_input(input_file_name);
  if (!error.empty()) {
    std::cerr << "Error: " << error << "\n";
    return 1;
  }
  const auto result = max_sequence::max_configuous_sequence(input, max_candy);
  if (result.valid()) {
    std::cout << "Start at home " << result.begin + 1 << " and go to home "
              << result.end + 1 << " to collect " << result.sum
              << " pieces of candy.\n";
  } else {
    std::cout << "No solution\n";
  }
  return 0;
}
#endif
