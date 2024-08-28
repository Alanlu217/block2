#pragma once

#include "game_state.hpp"

#include <string>

namespace SaveManager {

std::string saveToFile(std::string save_name, GameStateP state);

std::string loadFromFile(std::string save_name, GameStateP state);

std::string loadFromExternalFile(std::string save_name, GameStateP state);

std::string saveScoreToFile(std::string save_name, GameStateP state);

void loadDefault(GameStateP state);

} // namespace SaveManager