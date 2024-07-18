#pragma once

#include "entities/squircle.hpp"
#include "managers/resource_manager.hpp"
#include "raylib.h"
#include <optional>

class Dragger {
private:
  TextureP direction_arrow_texture;

  float temp_angle = 0;

  std::optional<Vector2> drag_init_pos;

public:
  Dragger();

  std::optional<Vector2> update(); // Returns dragged value if drag released

  void drawArrow(const Squircle &squircle);

  void drawCircle();
};
