#pragma once

#include "entities/background.hpp"
#include "entities/basic_platform.hpp"
#include "entities/dragger.hpp"
#include "entities/squircle.hpp"
#include "raylib.h"

#include <memory>
#include <string>

struct GameState {
  std::string name = "block";

  bool show_debug = false;

  float height = 0;

  Camera2D game_camera;

  struct {
    Squircle squircle;

    Dragger dragger;

    Background back_ground;

    std::vector<BasicPlatform> platforms;

  } entities;
};

typedef std::shared_ptr<GameState> GameStateP;
