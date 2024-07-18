#include "views/editor_view.hpp"

#include "game_state.hpp"
#include "imgui.h"
#include "managers/physics_manager.hpp"
#include "raylib.h"
#include "window.hpp"
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
};

void EditorView::update(const double deltaTime) {
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
  if (game_state->show_debug) {
    ImGui::Begin("EditorView");

    ImGui::DragFloat("Height:", &game_state->height);

    ImGui::End();
  }

  camera->target = {0, -static_cast<float>(game_state->height)};

  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &platform : *platforms) {
    platform.draw();
  }

  EndMode2D();
}

void EditorView::close() {}
