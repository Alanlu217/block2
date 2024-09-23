#pragma once

#include "managers/resource_manager.hpp"
#include <memory>

struct GameState;

class HeightDisplay {
private:
  FontP hud_font;

public:
  HeightDisplay();

  void draw(std::shared_ptr<GameState> state);
};