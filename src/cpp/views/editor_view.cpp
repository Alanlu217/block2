#include "views/editor_view.hpp"

#include "constants.hpp"
#include "entities/basic_platform.hpp"
#include "events/change_view_event.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/event_manager.hpp"
#include "managers/physics_manager.hpp"
#include "raylib.h"
#include "raymath.h"
#include "window.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

EditorView::EditorView(GameStateP state)
    : game_state(state), platforms(&state->entities.platforms),
      camera(&state->game_camera), back_ground(&state->entities.back_ground) {}

void EditorView::init() {
  *camera = Camera2D{};
  camera->target = {0, 0};
  camera->offset = {0, 0};
  camera->rotation = 0;
  camera->zoom = 1;

  game_state->height = 0;

  state = Idle;
  selected_platforms = {};
  mouse_drag_init = {};
};

void EditorView::update(const double deltaTime) {

  // Update height
  float movement = 0;
  if (float mouse_wheel = GetMouseWheelMove(); mouse_wheel != 0) {
    movement = std::copysign(std::abs(mouse_wheel), mouse_wheel);
  } else {
    if (IsKeyDown(KEY_UP)) {
      movement += 1;
    } else if (IsKeyDown(KEY_DOWN)) {
      movement -= 1;
    }
  }

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    game_state->height += movement * deltaTime * 4000;
  } else {
    game_state->height += movement * deltaTime * 1000;
  }
}

void EditorView::update_selection() {
  auto mouse_pos = win::getMousePos();
  mouse_pos.y += game_state->height;

  // Check for selection
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    bool selected_nothing = true;

    if (selected_platforms.size() == 1 &&
        CheckCollisionPointRec(GetMousePosition(), Rectangle{0, 0, 600, 55})) {
      return;
    }

    for (auto &platform : *platforms) {
      if (CheckCollisionPointRec(mouse_pos, platform.rect)) {
        selected_nothing = false;

        if (std::ranges::find(selected_platforms, &platform) !=
            selected_platforms.end()) {    // Platform already selected
          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Remove platform
            selected_platforms.erase(std::remove(selected_platforms.begin(),
                                                 selected_platforms.end(),
                                                 &platform),
                                     selected_platforms.end());

            state = Idle;

            return;
          } else { // Start Drag
            state = Dragging;
            mouse_drag_init = mouse_pos;

            return;
          }
        } else // Platform not selected

          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Add platform to selection
            selected_platforms.push_back(&platform);

            state = Idle;

            return;
          } else { // Set selection to platform
            selected_platforms.clear();
            selected_platforms.push_back(&platform);

            gui_rect[0] = platform.rect.x;
            gui_rect[1] = platform.rect.y;
            gui_rect[2] = platform.rect.width;
            gui_rect[3] = platform.rect.height;

            mouse_drag_init = mouse_pos;

            state = Dragging;

            return;
          }
      }
    }

    if (selected_nothing) {
      state = Selecting;
      mouse_drag_init = mouse_pos;

      return;
    }

  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

    if (state == Dragging) {
      state = Idle;
      mouse_drag_init = {};
    }

    if (mouse_pos.x == mouse_drag_init->x && mouse_pos.y == mouse_pos.y &&
        state == Selecting) { // Selection is a point
      selected_platforms.clear();
      state = Idle;
      mouse_drag_init = {};

      return;
    }

    if (state == Selecting) {
      const Rectangle selection_rect = {
          std::min(mouse_pos.x, mouse_drag_init->x),
          std::min(mouse_pos.y, mouse_drag_init->y),
          std::abs(mouse_pos.x - mouse_drag_init->x),
          std::abs(mouse_pos.y - mouse_drag_init->y)};

      bool selected_nothing = true;

      for (auto &platform : *platforms) {
        if (CheckCollisionRecs(selection_rect, platform.rect)) {
          selected_nothing = false;

          if (std::ranges::find(selected_platforms, &platform) !=
              selected_platforms.end()) {
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
              selected_platforms.erase(std::remove(selected_platforms.begin(),
                                                   selected_platforms.end(),
                                                   &platform),
                                       selected_platforms.end());
            }
          } else {
            selected_platforms.push_back(&platform);

            if (selected_platforms.size() == 1) {
              auto &platform = selected_platforms[0];

              gui_rect[0] = platform->rect.x;
              gui_rect[1] = platform->rect.y;
              gui_rect[2] = platform->rect.width;
              gui_rect[3] = platform->rect.height;
            }

            TraceLog(LOG_INFO, "Add Platform by Selection");
          }
        }
      }

      if (selected_nothing) {
        selected_platforms = {};
      }
    }
    mouse_drag_init = {};
  } else if (state == Dragging) {
    auto delta = Vector2Subtract(mouse_pos, *mouse_drag_init);
    mouse_drag_init = mouse_pos;

    for (auto platform : selected_platforms) {
      platform->rect.x += delta.x;
      platform->rect.y += delta.y;
    }

    if (selected_platforms.size() == 1) {
      auto &platform = selected_platforms[0];

      gui_rect[0] = platform->rect.x;
      gui_rect[1] = platform->rect.y;
      gui_rect[2] = platform->rect.width;
      gui_rect[3] = platform->rect.height;
    }
  }

  // Delete Platform
  if (IsKeyPressed(KEY_D)) {
    delete_selected_platforms();
  }

  // Add new platform
  if (IsKeyPressed(KEY_A)) {
    platforms->push_back({mouse_pos.x, mouse_pos.y, 100, 10});
  }

  // Copy platform dimensions
  if (IsKeyPressed(KEY_C)) {
    Rectangle rect = selected_platforms[0]->rect;
    copied_platform = rect;
  }

  // Pasts platform
  if (IsKeyPressed(KEY_V)) {
    selected_platforms.clear();

    if (copied_platform.has_value()) {
      platforms->emplace_back(mouse_pos.x, mouse_pos.y, copied_platform->width,
                              copied_platform->height);
    }
  }
}

void EditorView::delete_selected_platforms() {
  for (auto plat : selected_platforms) {
    auto pos =
        std::find_if(platforms->begin(), platforms->end(),
                     [plat](auto &platform) { return plat == &platform; });
    if (pos != platforms->end())
      platforms->erase(pos, platforms->end());
  }
  selected_platforms = {};
}

void EditorView::render(const double deltaTime) {

  update_selection();

  if (game_state->show_debug) {
    ImGui::Begin("EditorView");

    ImGui::DragFloat("Height:", &game_state->height);

    ImGui::Text("State: %d", state);

    ImGui::Text("Num Selected: %zu", selected_platforms.size());

    ImGui::End();
  }

  if (selected_platforms.size() == 1) {

    ImGui::SetNextWindowPos(ImVec2{0, 0});
    ImGui::SetNextWindowSize(ImVec2{320, 55});

    auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Platform Editor", NULL, flags);

    ImGui::DragFloat4("X, Y, W, H", gui_rect);

    gui_rect[2] = std::max(0.0f, gui_rect[2]);

    gui_rect[0] = std::max(0.0f, gui_rect[0]);
    gui_rect[0] =
        std::min(float(constants::window_width - gui_rect[2]), gui_rect[0]);

    selected_platforms[0]->rect.x = gui_rect[0];
    selected_platforms[0]->rect.y = gui_rect[1];
    selected_platforms[0]->rect.width = gui_rect[2];
    selected_platforms[0]->rect.height = gui_rect[3];

    ImGui::End();
  }

  ImGui::SetNextWindowPos(ImVec2{320, 0});
  ImGui::SetNextWindowSize(ImVec2{280, 55});
  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  ImGui::Begin("Editor", NULL, flags);

  if (ImGui::Button("Play")) {
    struct ChangeViewEvent event;
    event.new_view = "game";
    event.old_view = "editor";

    EventManager::triggerEvent(event);
  }
  ImGui::SameLine();
  ImGui::Button("Load");
  ImGui::SameLine();
  ImGui::Button("Save");
  ImGui::SameLine();
  ImGui::InputText("File", file_name, 50);
  ImGui::SameLine();

  ImGui::End();

  camera->target = {0, -static_cast<float>(game_state->height)};

  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &platform : *platforms) {
    platform.draw();
  }

  for (auto platform : selected_platforms) {
    win::drawRectangleLines(platform->rect, 2, GREEN);
  }

  if (mouse_drag_init.has_value()) {
    win::drawRectangleLines(
        Rectangle{std::min(mouse_drag_init->x, float(win::getMouseX())),
                  std::min(mouse_drag_init->y,
                           float(win::getMouseY() + game_state->height)),
                  std::abs(win::getMouseX() - mouse_drag_init->x),
                  std::abs(win::getMouseY() + game_state->height -
                           mouse_drag_init->y)},
        2, Color{255, 255, 255, 100});
  }

  EndMode2D();
}

void EditorView::close() {}
