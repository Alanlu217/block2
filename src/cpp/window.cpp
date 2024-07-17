#include "window.hpp"
#include "constants.hpp"

#include "raylib.h"

namespace win {

double toWindowUnits(double y, double height) {
  return -y + constants::WINDOW_HEIGHT - height;
}

void drawRectangle(Rectangle rect, Color color) {
  rect.y = toWindowUnits(rect.y, rect.height);
  DrawRectangleRec(rect, color);
}

void drawTexture(TextureP texture, Vector2 pos, Color color) {
  pos.y = toWindowUnits(pos.y, texture->height);
  texture->Draw(pos.x, pos.y, color);
}

} // namespace win