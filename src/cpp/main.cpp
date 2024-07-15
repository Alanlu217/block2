#include "app.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "raylib-cpp.hpp"
#include "rlImGui.h"

#include <iostream>

int main(void) {
  App app;

  while (app.isOpen()) {
    app.run();
  }
}