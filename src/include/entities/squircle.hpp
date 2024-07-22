#pragma once

#include "managers/resource_manager.hpp"

#include <raylib.h>

class Squircle {
private:
  TextureP squircle_texture;

public:
  Vector2 pos;
  Vector2 vel;
  float width = 64;

  bool grounded = false;

  Squircle();

  void draw();

  Rectangle getBounds();
};
