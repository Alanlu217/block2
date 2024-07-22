#pragma once

#include <raylib.h>

class BasicPlatform {
public:
  Rectangle rect;

  BasicPlatform();
  BasicPlatform(float x, float y, float width, float height);

  void draw();
};
