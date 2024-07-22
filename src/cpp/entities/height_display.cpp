#include "entities/height_display.hpp"
#include "raylib.h"
#include <string>

HeightDisplay::HeightDisplay() {
  height_font = ResourceManager::getFont("comfortaa.ttf");
}

void HeightDisplay::draw(float height, float max_height) {
  DrawTextEx(*height_font, std::to_string(int(height)).c_str(), {20, 20}, 46, 0,
             WHITE);
  DrawTextEx(*height_font, std::to_string(int(max_height)).c_str(), {20, 60},
             24, 0, WHITE);
}