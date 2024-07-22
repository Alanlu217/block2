#pragma once

#include "managers/resource_manager.hpp"

class HeightDisplay {
private:
  FontP height_font;

public:
  HeightDisplay();

  void draw(float height, float max_height);
};