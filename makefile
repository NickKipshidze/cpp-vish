# Compiler
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11

# Directories
SRC_DIR := src
BUILD_DIR := build

# Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
EXECUTABLE := vish

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
