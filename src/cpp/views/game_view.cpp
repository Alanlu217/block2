#include "views/game_view.hpp"

#include "game_state.hpp"
#include "imgui.h"
#include "managers/physics_manager.hpp"
#include "raylib.h"

GameView::GameView(GameStateP state)
    : game_state(state), platforms(&state->entities.platforms),
      squircle(&state->entities.squircle) {}

void GameView::init() {
  squircle->pos = {300.0f - float(squircle->width) / 2, 400};
  squircle->vel = {1000, 0};
};

void GameView::update(const double deltaTime) {
  physics::update(deltaTime, game_state);
}

void GameView::render(const double deltaTime) {
  if (game_state->show_debug) {
    ImGui::Begin("GameView");

    ImGui::Text("Height: %f", game_state->height);

    ImGui::Text("Squircle:\nPos: %f, %f\nVel: %f, %f", squircle->pos.x,
                squircle->pos.y, squircle->vel.x, squircle->vel.y);

    ImGui::End();
  }

  for (auto &platform : *platforms) {
    platform.draw();
  }

  squircle->draw();
}

void GameView::close() {}
