#include "app.hpp"

#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"
#include "events/close_window_event.hpp"
#include "events/event.hpp"
#include "events/toggle_debug_event.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/view_manager.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "views/game_view.hpp"
#include "views/start_view.hpp"

#include <iostream>
#include <memory>

App::App() {
  rlImGuiSetup(true);

  std::vector<BasicPlatform> &platforms = game_state.entities.platforms;
  platforms.push_back({0, 0, 600, 10});

  views["start"] = std::make_shared<StartView>();
  views["game"] = std::make_shared<GameView>(game_state);
  ViewManager::init("game", views);

  delta_update_time = 0;
  last_update_time = window.GetTime();
  last_render_time = window.GetTime();

  sleep_time = 0;

  show_debug = false;

  EventManager::addListener(CloseWindowEvent,
                            [](Event event) { CloseWindow(); });

  EventManager::addListener(ChangeViewEvent, [](Event event) {
    ViewManager::closeView(std::get<ChangeViewEvent>(event).old_view);
    ViewManager::addView(std::get<ChangeViewEvent>(event).new_view);
  });

  bool &debug = show_debug;
  EventManager::addListener(ToggleDebugEvent,
                            [&debug](Event event) { debug = !debug; });
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

    delta_update_time = current_time - last_update_time;

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

  if (show_debug) {
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %i", static_cast<int>(1 / delta_time));
    ImGui::Text("UPS: %i", static_cast<int>(1 / delta_update_time));

    if (ImGui::BeginListBox(
            "Views", ImVec2(-FLT_MIN - 35,
                            ViewManager::getView().size() *
                                    ImGui::GetTextLineHeightWithSpacing() +
                                2))) {
      for (const auto &item : ViewManager::getView()) {
        if (ImGui::Selectable(item.first.c_str())) {
          ViewManager::setView(item.first);
        }
      }

      ImGui::EndListBox();
    }

    ImGui::End();
  }

  rlImGuiEnd();
  window.EndDrawing();

  ResourceManager::clearUnused();
}

void App::update(double delta_time) {
  if (IsKeyPressed(KEY_BACKSLASH)) {
    struct ToggleDebugEvent event;
    EventManager::triggerEvent(event);
  }

  EventManager::update();
  ViewManager::update(window, delta_time);
}