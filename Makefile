# Directories
SRC_DIR := src
BIN_DIR := bin
DLL_DIR := dll

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -I"C:/msys64/mingw64/include"
LDFLAGS := -L"C:/msys64/mingw64/lib" -lsfml-graphics -lsfml-window -lsfml-system

# Source and target
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TARGET := $(BIN_DIR)/main.exe

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	@echo Copying DLLs...
	@xcopy /Y "$(DLL_DIR)\*.dll" "$(BIN_DIR)\"

run: all
	./$(TARGET)

clean:
	del /Q $(BIN_DIR)\*.exe 2>nul || rm -f $(BIN_DIR)/*.exe
