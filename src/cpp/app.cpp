#include "app.hpp"

#include "events/close_window_event.hpp"
#include "events/event.hpp"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/view_manager.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "views/start_view.hpp"

App::App() {
  rlImGuiSetup(true);

  views["start"] = std::make_shared<StartView>();
  ViewManager::init("start", views);

  delta_update_time = 0;
  last_update_time = window.GetTime();
  last_render_time = window.GetTime();

  sleep_time = 0;

  EventManager::addListener(CloseWindowEvent,
                            [](Event event) { CloseWindow(); });

  EventManager::addListener(ChangeViewEvent, [](Event event) {
    ViewManager::closeView(std::get<ChangeViewEvent>(event).old_view);
    ViewManager::addView(std::get<ChangeViewEvent>(event).new_view);
  });
}

App::~App() {
  rlImGuiShutdown();
  window.Close();
}

bool App::isOpen() { return !window.ShouldClose(); }

raylib::Window *App::getWindow() { return &window; }

void App::run() {
  double current_time = window.GetTime();

  double update_time_left =
      1.0 / target_ups - (current_time - last_update_time);
  double render_time_left =
      1.0 / target_fps - (current_time - last_render_time);

  if (update_time_left < render_time_left) {
    WaitTime(update_time_left);

    update(current_time - last_update_time);

    last_update_time = current_time;
  } else {
    WaitTime(render_time_left);

    render(current_time - last_render_time);

    last_render_time = current_time;
  }
}

void App::render(double delta_time) {
  window.BeginDrawing();
  rlImGuiBegin();

  window.ClearBackground();

  ViewManager::render(window, delta_time);

  rlImGuiEnd();
  window.EndDrawing();

  ResourceManager::clearUnused();
}

void App::update(double delta_time) {
  EventManager::update();
  ViewManager::update(window, delta_time);
}