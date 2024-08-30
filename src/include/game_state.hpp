#pragma once

#include "entities/background.hpp"
#include "entities/dragger.hpp"
#include "entities/hud.hpp"
#include "entities/objects/basic_platform.hpp"
#include "entities/squircle.hpp"

#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

struct GameState {
  std::string name = "default";

  bool show_debug = false;

  float height = 0;
  float max_height = 0;
  int jumps_made = 0;

  Camera2D game_camera;

  struct {
    Squircle squircle;

    Dragger dragger;

    HeightDisplay height_display;

    Background back_ground;
  } entities;

  std::vector<ObjectP> objects;
};

typedef std::shared_ptr<GameState> GameStateP;
