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
  origin.y = dest.height - origin.y;
  dest.y = toWindowUnits(dest.y, dest.height) + origin.y;

  // Still buggy atm, origin is moving image position
  DrawTexturePro(*texture, source, dest, origin, rotation, color);
}

Vector2 toGameUnits(Vector2 vec) {
  vec.y = -vec.y + constants::window_width;
  return vec;
}

Vector2 getMousePos() { return toGameUnits(GetMousePosition()); }

int getMouseX() { return GetMouseX(); }

int getMouseY() { return toWindowUnits(GetMouseY(), 0); }

} // namespace win
