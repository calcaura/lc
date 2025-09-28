WORKSPACE_BUILD_ROOT ?= /tmp
CXX_FLAGS_EXTRA ?= -g -O0 -fsanitize=address -fsanitize=undefined

# Compiler and flags
CXX=clang++
TIDY=clang-tidy
CXX_INC_PATHS=-I/workspace/libs
CXX_FLAGS=-std=c++20 -Wall -Wextra -Werror $(CXX_FLAGS_EXTRA) $(CXX_INC_PATHS)
CXX_LIBS=
CXX_LIBS_TEST=-lgtest -lgtest_main


# Output directories and files
OUTPUT_FOLDER=$(WORKSPACE_BUILD_ROOT)/$(notdir $(CURDIR))
OUTPUT_BIN=$(OUTPUT_FOLDER)/bin

# Source files
FILE_LIST=$(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,$(OUTPUT_FOLDER)/%.o,$(FILE_LIST))


$(OUTPUT_FOLDER)/%.o: %.cpp | prep
	$(TIDY) $< -- $(CXX_FLAGS)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

build: prep $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) -o $(OUTPUT_BIN) $(OBJ_FILES) $(CXX_LIBS) $(CXX_LIBS_TEST)


prep:
	mkdir -p $(OUTPUT_FOLDER)

test: build
	$(OUTPUT_BIN)

clean:
	rm -rf $(OUTPUT_FOLDER)

.PHONY: build prep test clean
.DEFAULT_GOAL := test