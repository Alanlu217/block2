#pragma once

#include "raylib.h"

class BasicPlatform {
private:
  Rectangle rect;

public:
  BasicPlatform();
  BasicPlatform(float x, float y, float width, float height);

  virtual ~BasicPlatform();

  virtual void draw();

  virtual void checkCollision();
};