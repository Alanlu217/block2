#pragma once

#include "managers/resource_manager.hpp"
#include "raylib.h"

class Squircle {
private:
  TextureP squircle_texture;

  Vector2 pos;

  int width = 64;

public:
  Squircle();
  ~Squircle();

  void draw();

  Vector2 getPosition();

  int getWidth();

  void setPosition(Vector2 pos);

  Rectangle getBounds();
};