#pragma once

#include "entities/background.hpp"
#include "entities/dragger.hpp"
#include "entities/hud.hpp"
#include "entities/objects/object.hpp"
#include "entities/squircle.hpp"

#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

struct GameState {
  std::string name = "default";

  std::string death_message = "Null";

  bool show_debug = false;
  bool height_should_increase = true;

  float height = 0;
  float max_height = 0;
  int jumps_made = 0;

  int double_jumps = 0;

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
