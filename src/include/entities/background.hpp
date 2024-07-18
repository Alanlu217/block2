#pragma once

#include "managers/resource_manager.hpp"

class Background {
private:
  TextureP background_texture;
  float pos1, pos2;

public:
  Background();

  void update(float height);

  void draw();
};
