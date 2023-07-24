CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra 

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
LIB_NAME = seeker_static_lib
LIB = $(LIB_DIR)/lib$(LIB_NAME).a

TARGET = $(OBJ_DIR)/libslib.a

all: $(LIB)

$(LIB): $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $(OBJECTS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB)
