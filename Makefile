#
# \file Makefile
# \author Kathi
# \date Created at: 2025-09-12
# \date Last modified at: 2025-09-12
# ---
#

#
# Compiler flags
#
CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -std=c++20

#
# Project structure
#
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
TARGET := $(BIN_DIR)/myfind

#
# Project files
#
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o, $(SRCS))

#
# Default target
#
.PHONY: clean all myfind 
all: clean myfind

#
# Debug build
#
debug: CXXFLAGS += -g -DDEBUG -O0
debug: clean $(TARGET)-debug

$(TARGET)-debug: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

#
# Release build
#
myfind: CXXFLAGS += -O2
myfind: $(TARGET)-release

$(TARGET)-release: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN_DIR)/myfind

#
# Object files build
#
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#
# Clean
#
clean:
	rm -rf $(BUILD_DIR)