WORKSPACE_BUILD_ROOT ?= /tmp
CXX_FLAGS_EXTRA ?= -g -O0 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

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
TIDY_FILES := $(patsubst %.cpp,$(OUTPUT_FOLDER)/%.tidy,$(FILE_LIST))

$(OUTPUT_FOLDER)/%.tidy: %.cpp | prep
	$(TIDY) $< -- $(CXX_FLAGS)
	@echo tidy > $@

$(OUTPUT_FOLDER)/%.o: %.cpp | prep
	$(CXX) $(CXX_FLAGS) -c $< -o $@

build: $(TIDY_FILES) $(OBJ_FILES) 
	$(CXX) $(CXX_FLAGS) -o $(OUTPUT_BIN) $(OBJ_FILES) $(CXX_LIBS) $(CXX_LIBS_TEST)

rebuild: clean build


tidy: $(TIDY_FILES)
	@true

prep:
	mkdir -p $(OUTPUT_FOLDER)

test: build
	ASAN_OPTIONS=detect_leaks=1 $(OUTPUT_BIN)

clean:
	rm -rf $(OUTPUT_FOLDER)

.PHONY: build rebuild prep test clean tidy
.DEFAULT_GOAL := test