#include "managers/physics_manager.hpp"

#include "constants.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include <iostream>

namespace physics {

void update(const double delta_time, GameStateP game_state) {

  // Update height
  // Currently linear, could be logarithmic later
  game_state->height += delta_time * constants::game::height_increase_per_s;

  // Make QOL reference to squircle
  Squircle &squircle = game_state->entities.squircle;

  // Update y velocity from gravity
  squircle.vel.y =
      squircle.vel.y - constants::squircle::gravity_acceleration * delta_time;

  // Update x velocity from air resistance
  squircle.vel.x =
      squircle.vel.x -
      squircle.vel.x *
          constants::squircle::air_friction_percent_decrease_per_s * delta_time;

  // Update squircle theoretical position
  squircle.pos = {
      static_cast<float>(delta_time * squircle.vel.x + squircle.pos.x),
      static_cast<float>(delta_time * squircle.vel.y + squircle.pos.y)};

  // Check for side wall collisions
  if (squircle.pos.x <= 0 + float(squircle.width) / 2) {
    squircle.pos.x = float(squircle.width) / 2;
    squircle.vel.x *= -1;
  }
  if (squircle.pos.x >= constants::window_width - squircle.width / 2) {
    squircle.pos.x = constants::window_width - squircle.width;
    squircle.vel.x *= -1;
  }
}

} // namespace physics
