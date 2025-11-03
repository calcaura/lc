# CPU Interview Assignment

In this assignment, you're tasked with optimizing the runtime of the computation
described below to the best of your ability on a CPU of your choice. A reference
implementation is provided as a basis.

The expected outcome is a report on your optimizations, their effect, and an
estimate of the maximum possible throughput. As a guideline, a 100x speedup over
the reference implementation is achievable in the provided benchmarks.

We will rate your assignment as:
 - 50% report
 - 50% implementation

Send us your results via E-mail by attaching a PDF of your report and your source code zipped with:

```sh
cd your/cpu/assignment/directory
tar cf ../cpu_assignment.tar.zstd benchmark CMakeLists.txt include src
```

## Task Description

Your task is to compute grouped aggregates. Assume a table consisting of data
items arranged into rows and columns. One column is designated as the "group
key".

First, assign each row to a group according to its group key. Then, aggregate
one or more "attribute" columns within each group. The required aggregate
functions are: `any_value` and `mean`.

The result is a table containing an aggregated value per (group, attribute)
combination. Each group is a row, each attribute is a column.

### Constraints

Allowed are any modifications to the operator and table implementations as well
as their APIs. You're free to use any of the included code and libraries (C++
`std`, `absl`, OpenMP), and to tune the compiler flags.

However, don't break the benchmark tests or compute incorrect results. The
provided benchmarks may not be modified, and no further libraries or
dependencies may be added.

Feel free to use the provided googletest skeleton inside `test/` to write own
tests to check your solution for correctness. These tests will be excluded from
grading.

## Setup Guide

The reference implementation is written in C++17 and built with CMake. We
recommend Linux for benchmarking.

To build the benchmark, run:

```sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make -j
```

Run `make test` inside the `build` folder to run your custom tests.`

The example query can then be benchmarked with:

```sh
./benchmark/group_by_bench --benchmark_time_unit=ms
```

Your optimized code is to be placed into `src/optimized_group_by.cpp`. Enable
this code path by changing the `group_by` function alias in
`include/db/group_by.hpp` as follows:

```c++
// constexpr auto group_by = reference_group_by;
constexpr auto group_by = optimized_group_by;
```

## FAQs

 - On Apple Macbooks, the reference solution takes more than 15 minutes to
   finish. This is due to Apple clang providing a slow `std::unordered_multimap`.
