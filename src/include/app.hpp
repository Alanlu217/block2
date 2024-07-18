#pragma once

#include "constants.hpp"
#include "game_state.hpp"
#include "managers/view_manager.hpp"

const int target_ups = 1200;
const int target_fps = 240;

class App {
private:
  ViewMap views;

  GameStateP game_state;

  double delta_update_time;
  double last_update_time;
  double last_render_time;

  double sleep_time;

  void update(double delta_time);
  void render(double delta_time);

public:
  App();
  ~App();

  bool isOpen();

  void run();
};
