#include "views/game_view.hpp"

#include "game_state.hpp"
#include "managers/physics_manager.hpp"

#include <imgui.h>
#include <raylib.h>

GameView::GameView(GameStateP state)
    : game_state(state), squircle(&state->entities.squircle),
      dragger(&state->entities.dragger), objects(&state->objects),
      back_ground(&state->entities.back_ground), camera(&state->game_camera) {}

void GameView::init() {
  squircle->pos = {300.0f - float(squircle->width) / 2, 100};
  squircle->vel = {0, 0};

  *camera = Camera2D{};
  camera->target = {0, 0};
  camera->offset = {0, 0};
  camera->rotation = 0;
  camera->zoom = 1;

  game_state->height = 0;
  game_state->jumps_made = 0;

  for (auto &object : game_state->objects) {
    object->init();
  }
};

void GameView::update(const double deltaTime) {
  if (paused) {
    return;
  }

  if (auto drag = dragger->update(); drag.has_value()) {
    TraceLog(LOG_INFO, "Drag at: X: %f, Y: %f", drag->x, drag->y);
    if (squircle->grounded || haxs) {
      squircle->vel = *drag;
      game_state->jumps_made++;
    }
  }

  physics::update(deltaTime, game_state);
}

void GameView::render(const double deltaTime) {
  if (game_state->show_debug) {
    ImGui::Begin("GameView");

    ImGui::DragFloat("Height:", &game_state->height);
    ImGui::DragFloat("Max Height:", &game_state->max_height);

    ImGui::Text("Squircle:\nPos: %f, %f\nVel: %f, %f\nGrounded: %d",
                squircle->pos.x, squircle->pos.y, squircle->vel.x,
                squircle->vel.y, squircle->grounded);

    if (ImGui::Button("Haxs")) {
      haxs = !haxs;
    }
    ImGui::SameLine();
    ImGui::Text(": %d", haxs);
    if (ImGui::Button("Height Haxs")) {
      game_state->height_should_increase = !game_state->height_should_increase;
    }
    ImGui::SameLine();

    ImGui::Text(": %d", game_state->height_should_increase);

    ImGui::End();
  }

  camera->target = {0, -static_cast<float>(game_state->height)};

  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &platform : *objects) {
    platform->draw();
  }

  squircle->draw();
  dragger->drawArrow(*squircle);
  EndMode2D();

  game_state->entities.height_display.draw(
      game_state->height, game_state->max_height, game_state->jumps_made);

  dragger->drawCircle();
}

void GameView::close() {}
