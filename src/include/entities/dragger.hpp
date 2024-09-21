#pragma once

#include "entities/squircle.hpp"
#include "managers/resource_manager.hpp"

#include <optional>
#include <raylib.h>

class Dragger {
private:
  TextureP direction_arrow_texture;

  float temp_angle = 0;

  std::optional<Vector2> drag_init_pos;

public:
  Dragger();

  std::optional<Vector2> update(); // Returns dragged value if drag released

  void reset();

  void drawArrow(const Squircle &squircle);

  void drawCircle();
};
