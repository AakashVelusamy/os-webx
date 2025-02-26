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

# Executables
SERVER_BIN = $(BIN_DIR)/webx
CLIENT_BIN = $(BIN_DIR)/client

# Default target (build both server and client)
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build the main server executable
$(SERVER_BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(SERVER_BIN)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the client separately
$(CLIENT_BIN): $(OBJ_DIR)/client.o
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)/client.o -o $(CLIENT_BIN)

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR)/*.o $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: clean all

