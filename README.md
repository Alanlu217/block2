# Block2

A hard rage game written in C++.

# Required Libraries

 - [raylib](https://github.com/raysan5/raylib)
 - [imgui](https://github.com/ocornut/imgui)
 - [rlImGui](https://github.com/raylib-extras/rlImGui)

# Steps to Build
Download the repository
```bash
git clone https://github.com/Alanlu217/block2.git
```
Pull dependencies
```bash
git submodule update --init
```
Generate the build files
```bash
cmake . -B build
```
Compile
```bash
cmake --build build -j
```
# Changelog

See [changelog.md](changelog.md)
