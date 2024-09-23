#include "entities/hud.hpp"
#include "game_state.hpp"
#include "raylib.h"
#include <string>

HeightDisplay::HeightDisplay() {
  hud_font = ResourceManager::getFont("comfortaa.ttf");
}

void HeightDisplay::draw(std::shared_ptr<GameState> state) {
  DrawTextEx(*hud_font, std::to_string(int(state->height)).c_str(), {20, 20},
             46, 0, WHITE);
  DrawTextEx(*hud_font, std::to_string(int(state->max_height)).c_str(),
             {20, 60}, 24, 0, WHITE);

  std::string jumpText = "Jumps: " + std::to_string(state->jumps_made);
  DrawTextEx(*hud_font, jumpText.c_str(), {20, 140}, 24, 0, WHITE);

  std::string doubleJumpText =
      "Doubles: " + std::to_string(state->double_jumps);
  DrawTextEx(*hud_font, doubleJumpText.c_str(), {20, 100}, 24, 0, WHITE);
}