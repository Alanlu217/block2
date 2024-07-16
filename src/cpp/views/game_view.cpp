#include "views/game_view.hpp"

#include "game_state.hpp"
#include "raylib.h"

GameView::GameView(GameState &state)
    : platforms(state.entities.platforms), squircle(state.entities.squircle) {}

void GameView::init() {
  squircle.setPosition({300.0f - squircle.getWidth() / 2, 400});
};

void GameView::update(raylib::Window &window, const double deltaTime) {}

void GameView::render(raylib::Window &window, const double deltaTime) {
  for (auto &platform : platforms) {
    platform.draw(window);
  }

  squircle.draw(window);
}

void GameView::close() {}
