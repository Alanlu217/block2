#include "window.hpp"
#include "constants.hpp"

#include "raylib.h"

namespace win {

double toWindowUnits(double y, double height) {
  return -y + constants::window_height - height;
}

void drawRectangle(Rectangle rect, Color color) {
  rect.y = toWindowUnits(rect.y, rect.height);
  DrawRectangleRec(rect, color);
}

void drawTexture(TextureP texture, Vector2 pos, Color color) {
  pos.y = toWindowUnits(pos.y, texture->height);
  DrawTexture(*texture, pos.x, pos.y, color);
}

Vector2 toGameUnits(Vector2 vec) {
  vec.y = -vec.y + constants::window_width;
  return vec;
}

} // namespace win
