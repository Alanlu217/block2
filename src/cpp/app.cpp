#include "app.hpp"

#include "events/event.hpp"
#include "events/toggle_debug_event.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/input_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/save_manager.hpp"
#include "managers/view_manager.hpp"
#include "views/death_view.hpp"
#include "views/editor_view.hpp"
#include "views/game_view.hpp"
#include "views/start_view.hpp"
#include "window.hpp"

#include <imgui.h>
#include <memory>
#include <raylib.h>
#include <rlImGui.h>
#include <thread>

App::App() {
  SetTraceLogLevel(LOG_NONE);

  InitWindow(constants::window_width, constants::window_height,
             constants::window_title.c_str());

  rlImGuiSetup(true);

  auto &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigDockingWithShift = true;

  Input::initialise();

  SetExitKey(KEY_NULL);

  game_state = std::make_shared<GameState>();

  SaveManager::loadDefault(game_state);

  views["start"] = std::make_shared<StartView>(game_state);
  views["game"] = std::make_shared<GameView>(game_state);
  views["editor"] = std::make_shared<EditorView>(game_state);
  views["death"] = std::make_shared<DeathView>(game_state);
  ViewManager::init("start", views);

  next_render_time = now();
  next_update_time = now();
  last_update = now();
  last_render = now();

  setTargetFPS(target_fps);
  setTargetUPS(target_ups);

  EventManager::addListener(CloseWindowEvent,
                            [](Event event) { CloseWindow(); });

  EventManager::addListener(ChangeViewEvent, [](Event event) {
    ViewManager::closeView(std::get<ChangeViewEvent>(event).old_view);
    ViewManager::addView(std::get<ChangeViewEvent>(event).new_view);
  });

  EventManager::addListener(StartTestEvent, [&](Event event) {
    Vector2 pos = std::get<StartTestEvent>(event).squircle_pos;
    float height = std::get<StartTestEvent>(event).height;
    ViewManager::closeView(std::get<StartTestEvent>(event).old_view);
    ViewManager::addView("game");
    game_state->entities.squircle.pos = pos;
    game_state->height = height;
  });

  GameStateP state = game_state;
  EventManager::addListener(ToggleDebugEvent, [state](Event event) {
    state->show_debug = !state->show_debug;
  });
}

App::~App() {
  SaveManager::saveScoreToFile(game_state->name, game_state);
  rlImGuiShutdown();
  CloseWindow();
}

void App::setTargetFPS(double fps) {
  using namespace std::chrono_literals;
  render_interval =
      std::chrono::duration_cast<std::chrono::nanoseconds>((1.0 / fps) * 1s);
}

void App::setTargetUPS(double ups) {
  using namespace std::chrono_literals;
  update_interval =
      std::chrono::duration_cast<std::chrono::nanoseconds>((1.0 / ups) * 1s);
}

bool App::isOpen() { return !WindowShouldClose(); }

void App::run() {
  using namespace std::chrono_literals;

  if (next_render_time < next_update_time) {
    std::this_thread::sleep_until(next_render_time);
    next_render_time += render_interval;
    auto n = now();
    if (n > next_render_time) {
      next_render_time = n;
    }

    std::chrono::nanoseconds delta =
        std::chrono::duration_cast<std::chrono::nanoseconds>(n - last_render);
    last_render = n;
    render(delta.count() / 1000000000.0);

  } else {
    std::this_thread::sleep_until(next_update_time);
    next_update_time += update_interval;
    auto n = now();
    if (n > next_update_time) {
      next_update_time = n;
    }

    std::chrono::nanoseconds delta =
        std::chrono::duration_cast<std::chrono::nanoseconds>(n - last_update);
    last_update = n;
    update(delta.count() / 1000000000.0);
    last_update_delta = delta;
  }
}

void App::render(double delta_time) {
  Input::update();
  Input::startRender();

  BeginDrawing();
  rlImGuiBegin();

  ClearBackground(Color{46, 46, 46, 1});

  ViewManager::render(delta_time);
  // std::cout << delta_time << "\n";

  if (game_state->show_debug) {
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %i", static_cast<int>(1 / delta_time));
    ImGui::Text("UPS: %i", static_cast<int>(
                               1 / (last_update_delta.count() / 1000000000.0)));

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

  Input::resetRender();
  ResourceManager::clearUnused();
}

void App::update(double delta_time) {
  Input::startUpdate();
  if (Input::isKeyPressed(KEY_BACKSLASH)) {
    struct ToggleDebugEvent event;
    EventManager::triggerEvent(event);
  }

  EventManager::update();
  ViewManager::update(delta_time);

  Input::resetUpdate();
}
