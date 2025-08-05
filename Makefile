RAYLIB_PATH = /opt/homebrew/opt/raylib

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD -MP -Iinclude -I$(RAYLIB_PATH)/include
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
DEP = $(OBJ:.o=.d)

TARGET = main

all: $(TARGET)

# Compile rule with dependency generation
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all object files into the final binary
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Include all auto-generated .d dependency files
-include $(DEP)

clean:
	rm -rf build $(TARGET)

