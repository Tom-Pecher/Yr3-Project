
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Project structure
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/env_sim

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# SFML Libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Linking target executable with SFML
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)


# Compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
