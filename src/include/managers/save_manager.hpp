#pragma once

#include "game_state.hpp"
#include <string>

namespace SaveManager {

std::string saveToFile(std::string file_name, GameStateP state);

std::string loadFromFile(std::string file_name, GameStateP state);

} // namespace SaveManager