#include "entities/basic_platform.hpp"

#include "constants.hpp"

#include "raylib.h"

BasicPlatform::BasicPlatform() { rect = {0, 0, 0, 0}; }
BasicPlatform::BasicPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

BasicPlatform::~BasicPlatform() {}

void BasicPlatform::draw(raylib::Window &window) {
  Rectangle temp = rect;

  temp.y = -temp.y + constants::WINDOW_HEIGHT - temp.height;
  DrawRectangleRec(temp, WHITE);
}

void BasicPlatform::checkCollision() {}