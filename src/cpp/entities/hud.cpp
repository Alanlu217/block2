#include "entities/hud.hpp"
#include "raylib.h"
#include <string>

HeightDisplay::HeightDisplay() {
  hud_font = ResourceManager::getFont("comfortaa.ttf");
}

void HeightDisplay::draw(float height, float max_height, int jumps) {
  DrawTextEx(*hud_font, std::to_string(int(height)).c_str(), {20, 20}, 46, 0,
             WHITE);
  DrawTextEx(*hud_font, std::to_string(int(max_height)).c_str(), {20, 60}, 24,
             0, WHITE);

  std::string jumpText = "Jumps: " + std::to_string(jumps);
  Vector2 size = MeasureTextEx(*hud_font, jumpText.c_str(), 24, 0);

  DrawTextEx(*hud_font, jumpText.c_str(), {20, 100}, 24, 0, WHITE);
}