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

void drawRectangleLines(Rectangle rect, float width, Color color) {
  rect.y = toWindowUnits(rect.y, rect.height);
  DrawRectangleLinesEx(rect, width, color);
}

void drawCircle(Vector2 pos, float radius, Color color) {
  DrawCircle(pos.x, toWindowUnits(pos.y, 0), radius, color);
}

void drawTexture(TextureP texture, Vector2 pos, float rotation, float scale,
                 Color color) {
  pos.y = toWindowUnits(pos.y, texture->height);
  DrawTextureEx(*texture, pos, rotation, scale, color);
}

void drawTexturePro(TextureP texture, Rectangle source, Rectangle dest,
                    Vector2 origin, float rotation, Color color) {
  dest.y = toWindowUnits(dest.y, dest.height) + 2 * origin.y;
  origin.y = dest.height - origin.y;

  DrawTexturePro(*texture, source, dest, origin, rotation, color);
}

Vector2 toGameUnits(Vector2 vec) {
  vec.y = -vec.y + constants::window_width;
  return vec;
}

Vector2 getMousePos() {
  return {static_cast<float>(getMouseX()), static_cast<float>(getMouseY())};
}

int getMouseX() { return GetMouseX(); }

int getMouseY() { return toWindowUnits(GetMouseY(), 0); }

} // namespace win
