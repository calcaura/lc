WORKSPACE_BUILD_ROOT ?= /tmp
CXX_FLAGS_EXTRA ?= -g -O0 -fsanitize=address -fsanitize=undefined

# Compiler and flags
CXX=clang++
TIDY=clang-tidy
CXX_INC_PATHS=-I/workspace/libs
CXX_FLAGS=-std=c++20 -Wall -Wextra -Werror $(CXX_FLAGS_EXTRA) $(CXX_INC_PATHS)
CXX_LIBS=
CXX_LIBS_TEST=-lgtest -lgtest_main