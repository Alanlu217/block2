#pragma once

#include "raylib-cpp.hpp"
#include "raylib.h"

class BasicPlatform {
private:
  Rectangle rect;

public:
  BasicPlatform();
  BasicPlatform(float x, float y, float width, float height);

  virtual ~BasicPlatform();

  virtual void draw(raylib::Window &window);

  virtual void checkCollision();
};