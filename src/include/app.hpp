#pragma once

#include "managers/view_manager.hpp"
#include "raylib-cpp.hpp"

const int target_ups = 360;
const int target_fps = 120;

class App {
private:
  raylib::Window window = raylib::Window(600, 800, "block");

  ViewMap views;

  double delta_update_time;
  double last_update_time;
  double last_render_time;

  double sleep_time;

  bool show_debug;

  void update(double delta_time);
  void render(double delta_time);

public:
  App();
  ~App();

  bool isOpen();

  raylib::Window *getWindow();

  void run();
};