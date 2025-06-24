# Directories
SRC_DIR := src
BIN_DIR := bin
DLL_DIR := dll
INCLUDE_DIRS := include glm

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall $(foreach dir,$(INCLUDE_DIRS),-I$(dir))
LDFLAGS := -L"C:/msys64/mingw64/lib" -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

# Source and target
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(SRC_DIR)/gl.c
TARGET := $(BIN_DIR)/main.exe

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	@echo Copying DLLs...
	@xcopy /Y "$(DLL_DIR)\*.dll" "$(BIN_DIR)\" >nul 2>&1

run: all
	$(CXX) -v $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	./$(TARGET)

clean:
	-del /Q $(BIN_DIR)\*.exe 2>nul || rm -f $(BIN_DIR)/*.exe
