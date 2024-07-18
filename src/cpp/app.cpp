#include "app.hpp"

#include "entities/basic_platform.hpp"
#include "events/event.hpp"
#include "events/toggle_debug_event.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/view_manager.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "views/death_view.hpp"
#include "views/game_view.hpp"
#include "views/start_view.hpp"
#include "window.hpp"

#include <memory>

App::App() {
  InitWindow(constants::window_width, constants::window_height,
             constants::window_title.c_str());
  rlImGuiSetup(true);

  game_state = std::make_shared<GameState>();

  std::vector<BasicPlatform> &platforms = game_state->entities.platforms;
  platforms.push_back({0, 0, 600, 10});
  platforms.push_back({40, 200, 200, 10});
  platforms.push_back({300, 600, 200, 10});

  views["start"] = std::make_shared<StartView>();
  views["game"] = std::make_shared<GameView>(game_state);
  views["death"] = std::make_shared<DeathView>();
  ViewManager::init("start", views);

  delta_update_time = 0;
  last_update_time = GetTime();
  last_render_time = GetTime();

  sleep_time = 0;

  EventManager::addListener(CloseWindowEvent,
                            [](Event event) { CloseWindow(); });

  EventManager::addListener(ChangeViewEvent, [](Event event) {
    ViewManager::closeView(std::get<ChangeViewEvent>(event).old_view);
    ViewManager::addView(std::get<ChangeViewEvent>(event).new_view);
  });

  GameStateP state = game_state;
  EventManager::addListener(ToggleDebugEvent, [state](Event event) {
    state->show_debug = !state->show_debug;
  });
}

App::~App() {
  rlImGuiShutdown();
  CloseWindow();
}

bool App::isOpen() { return !WindowShouldClose(); }

void App::run() {
  double current_time = GetTime();

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
  BeginDrawing();
  rlImGuiBegin();

  ClearBackground(Color{46, 46, 46, 1});

  ViewManager::render(delta_time);

  if (game_state->show_debug) {
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

    ImGui::Text("Loaded Resources:\n%s",
                ResourceManager::listLoadedResources().c_str());

    ImGui::Text("Mouse Pos Window : %d, %d", GetMouseX(), GetMouseY());
    ImGui::Text("Mouse Pos Game   : %d, %d", win::getMouseX(),
                win::getMouseY());

    ImGui::End();
  }

  rlImGuiEnd();
  EndDrawing();

  ResourceManager::clearUnused();
}

void App::update(double delta_time) {
  if (IsKeyPressed(KEY_BACKSLASH)) {
    struct ToggleDebugEvent event;
    EventManager::triggerEvent(event);
  }

  EventManager::update();
  ViewManager::update(delta_time);
}
