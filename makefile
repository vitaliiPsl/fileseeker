CXX := g++
CXXFLAGS := -std=c++17 -Wall

SRC_DIR := src
BUILD_DIR := build
LIB_DIR := slib/lib
INCLUDE_DIR := slib/src/include 

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := fileseeker

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -L$(LIB_DIR) -lseeker_static_lib -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)