#include "views/game_view.hpp"

#include "game_state.hpp"
#include "imgui.h"
#include "managers/physics_manager.hpp"
#include "raylib.h"
#include "window.hpp"

GameView::GameView(GameStateP state)
    : game_state(state), platforms(&state->entities.platforms),
      squircle(&state->entities.squircle), dragger(&state->entities.dragger),
      camera(&state->game_camera), back_ground(&state->entities.back_ground) {}

void GameView::init() {
  squircle->pos = {300.0f - float(squircle->width) / 2, 400};
  squircle->vel = {0, 0};

  *camera = Camera2D{};
  camera->target = {0, 0};
  camera->offset = {0, 0};
  camera->rotation = 0;
  camera->zoom = 1;

  game_state->height = 0;
};

void GameView::update(const double deltaTime) {
  if (auto drag = dragger->update(); drag.has_value()) {
    TraceLog(LOG_INFO, "Drag at: X: %f, Y: %f", drag->x, drag->y);
    squircle->vel = *drag;
  }

  physics::update(deltaTime, game_state);
}

void GameView::render(const double deltaTime) {
  if (game_state->show_debug) {
    ImGui::Begin("GameView");

    ImGui::DragFloat("Height:", &game_state->height);

    ImGui::Text("Squircle:\nPos: %f, %f\nVel: %f, %f\nGrounded: %d",
                squircle->pos.x, squircle->pos.y, squircle->vel.x,
                squircle->vel.y, squircle->grounded);

    ImGui::End();
  }

  camera->target = {0, -static_cast<float>(game_state->height)};

  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &platform : *platforms) {
    platform.draw();
  }

  squircle->draw();
  dragger->drawArrow(*squircle);
  EndMode2D();

  dragger->drawCircle();
}

void GameView::close() {}
