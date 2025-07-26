# 3D GPU Particle Simulator
A high-performance, GPU-accelerated 3D particle simulation written in modern C++ using OpenGL 4.6 compute shaders and SFML for input and window management. 
The simulator supports spatial partitioning and interactive camera controls for visualizing large-scale particle interactions.

## Features
- GPU-based particle simulation using OpenGL compute shaders
- Spatial partitioning with uniform grid and cell hashing
- Simple collision handling and particle repulsion
- Real-time rendering and camera controls (keyboard)
- SFML-based windowing and input management

## Requirements
Before building, make sure the following are installed:

### Compiler & Build Tools
- **[MSYS2](https://www.msys2.org/)** (recommended)
- Use `mingw64` environment (not `ucrt64` or `clang64`)
- `g++` (installed via `pacman -S mingw-w64-x86_64-gcc`)
- `make` (optional, for building with provided Makefile)

### Libraries
- **[SFML 2.6.1](https://www.sfml-dev.org/)** (installed via MSYS2)
  ```bash
  pacman -S mingw-w64-x86_64-sfml

## Build Instructions
### Option 1: Using Makefile without make or SFML installed (MYSYS2 / MinGW64)
```pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sfml make```bash
```cd 3D-Particle-Simulator```bash
```make```bash

The executable will  be place in `bin/main.exe`. DLLs will be copied automatically

### Option 2: SFML installed, but compiled with  out make
```g++ -std=c++17 -Wall -Iinclude -Iglm src/*.cpp src/gl.c -o bin/main.exe \
-L"C:/msys64/mingw64/lib" -lsfml-graphics -lsfml-window -lsfml-system -lopengl32```bash

## Run Instructions
``` ./bin/main.exe ``` bash

## Controls
- `W/A/S/D` or arrow keys for movement
- `+/-` to zoom in/out

## Notes
- Requires an OpenGL 4.6+ capable GPU
- Make sure `glad` or your OpenGL loader is initialized correctly
- Particle logic is performed entirely on the GPU using compute shaders
