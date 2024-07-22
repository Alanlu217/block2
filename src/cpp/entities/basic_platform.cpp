#include "entities/basic_platform.hpp"

#include "constants.hpp"
#include "window.hpp"

#include <raylib.h>

BasicPlatform::BasicPlatform() { rect = {0, 0, 0, 0}; }
BasicPlatform::BasicPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

void BasicPlatform::draw() { win::drawRectangle(rect, WHITE); }
