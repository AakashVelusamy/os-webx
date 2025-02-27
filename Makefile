# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -pthread -std=c++11 -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Ensure necessary directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Source Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Executable
SERVER_BIN = $(BIN_DIR)/webx

# Default target (build server)
all: $(SERVER_BIN)

# Build the main server executable
$(SERVER_BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(SERVER_BIN)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR)/*.o $(SERVER_BIN)

.PHONY: clean all

