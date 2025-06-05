# test/Makefile

CXX := g++
CXXFLAGS := -std=c++20 -g -I../src

SRC_DIR := ../src
TEST_DIR := .
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TEST_DIR)/*.cpp)

# Replace .cpp with .o and prefix with BIN_DIR
OBJS := $(patsubst %.cpp,$(BIN_DIR)/%.o,$(notdir $(SRCS)))

TARGET := $(BIN_DIR)/bitonic_network_test

all: $(TARGET)

# Create BIN_DIR if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp into BIN_DIR/.o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp |$(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(TEST_DIR)/%.cpp |$(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
