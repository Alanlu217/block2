#pragma once

#include "constants.hpp"
#include "game_state.hpp"
#include "managers/view_manager.hpp"

#include <chrono>
using namespace std::chrono_literals;

const int target_ups = 1200;
const int target_fps = 240;

class App {
private:
  ViewMap views;

  GameStateP game_state;

  typedef std::chrono::time_point<std::chrono::steady_clock> time_point;
  time_point now() { return std::chrono::steady_clock::now(); }

  time_point next_update_time, next_render_time;
  time_point last_update, last_render;
  std::chrono::nanoseconds update_interval, render_interval;

  std::chrono::nanoseconds last_update_delta = 0ns;

  void setTargetFPS(double fps);
  void setTargetUPS(double ups);

  void update(double delta_time);
  void render(double delta_time);

public:
  App();
  ~App();

  bool isOpen();

  void run();
};
