#include "managers/physics_manager.hpp"

#include "constants.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include <iostream>

namespace physics {

void update(const double delta_time, GameStateP game_state) {
  game_state->height += delta_time * constants::game::height_increase_per_s;

  Squircle &squircle = game_state->entities.squircle;

  squircle.vel.y =
      squircle.vel.y - constants::squircle::gravity_acceleration * delta_time;

  squircle.vel.x =
      squircle.vel.x -
      squircle.vel.x *
          constants::squircle::air_friction_percent_decrease_per_s * delta_time;

  squircle.pos = {
      static_cast<float>(delta_time * squircle.vel.x + squircle.pos.x),
      static_cast<float>(delta_time * squircle.vel.y + squircle.pos.y)};
}

} // namespace physics
