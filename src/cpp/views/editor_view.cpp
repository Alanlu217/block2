#include "views/editor_view.hpp"

#include "entities/basic_platform.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/physics_manager.hpp"
#include "raylib.h"
#include "window.hpp"
#include <algorithm>
#include <cmath>

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

    for (auto &platform : *platforms) {
      TraceLog(LOG_INFO,
               "Mouse: X: %f Y: %f\n      Rect: X: %f Y: %f W: %f H: %f",
               mouse_pos.x, mouse_pos.y, platform.rect.x, platform.rect.y,
               platform.rect.width, platform.rect.height);
      if (CheckCollisionPointRec(mouse_pos, platform.rect)) {
        TraceLog(LOG_INFO, "Collide");
        selected_nothing = false;

        if (std::ranges::find(selected_platforms, &platform) !=
            selected_platforms.end()) {    // Platform already selected
          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Remove platform
            selected_platforms.erase(std::remove(selected_platforms.begin(),
                                                 selected_platforms.end(),
                                                 &platform),
                                     selected_platforms.end());

            state = Idle;

            TraceLog(LOG_INFO, "Removed Platform");

            return;
          } else { // Start Drag
            state = Dragging;
            mouse_drag_init = mouse_pos;

            TraceLog(LOG_INFO, "Start Movement Drag");
            return;
          }
        } else // Platform not selected

          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Add platform to selection
            selected_platforms.push_back(&platform);

            state = Idle;

            TraceLog(LOG_INFO, "Add Platform");

            return;
          } else { // Set selection to platform
            selected_platforms.clear();
            selected_platforms.push_back(&platform);

            state = Idle;

            TraceLog(LOG_INFO, "Clear and Set Platform");

            return;
          }
      }
    }

    if (selected_nothing) {
      state = Selecting;
      mouse_drag_init = mouse_pos;

      TraceLog(LOG_INFO, "Start Selection Drag");

      return;
    }

  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

    if (mouse_pos.x == mouse_drag_init->x && mouse_pos.y == mouse_pos.y &&
        state == Selecting) { // Selection is a point
      selected_platforms.clear();
      state = Idle;
      mouse_drag_init = {};

      TraceLog(LOG_INFO, "Deselect All");

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
              TraceLog(LOG_INFO, "Remove Platform by Selection");
            }
          } else {
            selected_platforms.push_back(&platform);

            TraceLog(LOG_INFO, "Add Platform by Selection");
          }
        }
      }

      if (selected_nothing) {
        selected_platforms = {};
      }
    }
    mouse_drag_init = {};
  }
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
