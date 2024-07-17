#pragma once

#include "managers/resource_manager.hpp"
#include "raylib.h"

class Squircle {
private:
  TextureP squircle_texture;

public:
  Vector2 pos;
  Vector2 vel;
  int width = 64;

  Squircle();
  ~Squircle();

  void draw();

  Rectangle getBounds();
};
