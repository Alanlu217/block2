#include "imgui.h"
#include "raylib-cpp.hpp"
#include "rlImGui.h"

int main(void) {
  const int screenWidth = 600;
  const int screenHeight = 800;

  raylib::Window window(screenWidth, screenHeight, "block");
  rlImGuiSetup(true);

  while (!window.ShouldClose()) {
    window.BeginDrawing();
    rlImGuiBegin();

    window.ClearBackground(BLACK);

    rlImGuiEnd();
    window.EndDrawing();
  }

  rlImGuiShutdown();
  window.Close();

  return 0;
}