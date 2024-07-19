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

  selected_platforms = {};
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

void EditorView::render(const double deltaTime) {
  // Check for selection
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    auto mouse_pos = win::getMousePos();
    mouse_drag_init = mouse_pos;

    mouse_pos.y += game_state->height;

    bool selected_nothing = true;

    for (auto &platform : *platforms) {
      if (CheckCollisionPointRec(mouse_pos, platform.rect)) {
        selected_nothing = false;

        if (std::ranges::find(selected_platforms, &platform) !=
                selected_platforms.end() &&
            IsKeyDown(KEY_LEFT_SHIFT)) {
          selected_platforms.erase(std::remove(selected_platforms.begin(),
                                               selected_platforms.end(),
                                               &platform),
                                   selected_platforms.end());
        } else if (selected_platforms.size() != 0 &&
                   IsKeyDown(KEY_LEFT_SHIFT)) {
          selected_platforms.push_back(&platform);
        } else {
          selected_platforms.clear();
          selected_platforms.push_back(&platform);
        }
      }
    }

    if (selected_nothing && IsKeyUp(KEY_LEFT_SHIFT)) {
      selected_platforms = {};
    }
  } else if (auto mouse_pos = win::getMousePos();
             IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
             mouse_drag_init.has_value() && mouse_pos.x != mouse_drag_init->x &&
             mouse_pos.y != mouse_drag_init->y) {
    mouse_pos.y += game_state->height;

    mouse_drag_init->y += game_state->height;

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
          selected_platforms.erase(std::remove(selected_platforms.begin(),
                                               selected_platforms.end(),
                                               &platform),
                                   selected_platforms.end());
        } else {
          selected_platforms.push_back(&platform);
        }
      }
    }

    if (selected_nothing) {
      selected_platforms = {};
    }
  }

  if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
    mouse_drag_init = {};
  }

  if (game_state->show_debug) {
    ImGui::Begin("EditorView");

    ImGui::DragFloat("Height:", &game_state->height);

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

  EndMode2D();

  if (mouse_drag_init.has_value()) {
    win::drawRectangleLines(
        Rectangle{std::min(mouse_drag_init->x, float(win::getMouseX())),
                  std::min(mouse_drag_init->y, float(win::getMouseY())),
                  std::abs(win::getMouseX() - mouse_drag_init->x),
                  std::abs(win::getMouseY() - mouse_drag_init->y)},
        2, Color{255, 255, 255, 100});
  }
}

void EditorView::close() {}
