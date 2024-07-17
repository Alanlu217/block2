#pragma once

#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"

#include <memory>
#include <string>

struct GameState {
  std::string name = "block";

  struct {
    Squircle squircle;

    std::vector<BasicPlatform> platforms;

  } entities;
};

typedef std::shared_ptr<GameState> GameStateP;