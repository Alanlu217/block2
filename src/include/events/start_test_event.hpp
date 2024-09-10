#pragma once

#include "raylib.h"
#include <string>

struct StartTestEvent {
  std::string old_view;

  Vector2 squircle_pos;
  float height;
};