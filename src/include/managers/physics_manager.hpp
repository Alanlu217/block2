#pragma once

#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"

#include <vector>

namespace physics {

void update(const double delta_time, GameStateP game_state);

}
