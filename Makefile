CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/redis

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
