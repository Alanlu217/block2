#include "views/game_view.hpp"

#include "game_state.hpp"
#include "raylib.h"

GameView::GameView(GameStateP state)
    : platforms(&state->entities.platforms),
      squircle(&state->entities.squircle) {}

void GameView::init() {
  squircle->setPosition({300.0f - float(squircle->getWidth()) / 2, 400});
};

void GameView::update(const double deltaTime) {}

void GameView::render(const double deltaTime) {
  for (auto &platform : *platforms) {
    platform.draw();
  }

  squircle->draw();
}

void GameView::close() {}
