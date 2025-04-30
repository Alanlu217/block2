# Block2

A hard rage game written in C++.

# Build
Download the repository with
```bash
git clone https://github.com/Alanlu217/block2.git
```
Then pull dependencies
```bash
git submodule update --init
```
To generate the build files run
```bash
cmake . -B build
```
then compile with
```bash
cmake --build build -j
```

# Used Libraries

 - [raylib](https://github.com/raysan5/raylib)
 - [imgui](https://github.com/ocornut/imgui)
 - [rlImGui](https://github.com/raylib-extras/rlImGui)

# Changelog

See [changelog.md](changelog.md)
