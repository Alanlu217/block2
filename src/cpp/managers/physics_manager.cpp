#include "managers/physics_manager.hpp"

#include "constants.hpp"
#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"
#include "events/change_view_event.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "window.hpp"

#include <algorithm>
#include <iostream>
#include <raylib.h>

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
  if (squircle.pos.x <= 0) {
    squircle.pos.x = 0;
    squircle.vel.x *= -1;
  }
  if (squircle.pos.x >= constants::window_width - squircle.width) {
    squircle.pos.x = constants::window_width - squircle.width;
    squircle.vel.x *= -1;
  }

  // Check for platform collisions
  for (BasicPlatform &platform : game_state->entities.platforms) {
    Rectangle &bounds = platform.rect;

    // Collision detected
    if (CheckCollisionRecs(bounds, squircle.getBounds())) {
      int leftOverlap = squircle.pos.x + squircle.width - bounds.x;
      int rightOverlap = bounds.x + bounds.width - squircle.pos.x;
      int topOverlap = bounds.y + bounds.height - squircle.pos.y;
      int bottomOverlap = squircle.pos.y + squircle.width - bounds.y;

      // Find the direction with minimum overlap distance
      int minOverlap =
          std::min({leftOverlap, rightOverlap, topOverlap, bottomOverlap});

      squircle.grounded = false;

      if (minOverlap == topOverlap) {
        squircle.pos.y = bounds.y + bounds.height;
        squircle.vel.y = abs(squircle.vel.y *
                             constants::squircle::bounce_velocity_reduction);

        // If is idle, adjust height
        game_state->height +=
            std::max((squircle.pos.y - game_state->height) - 400, 0.0f) *
            delta_time * constants::game::idle_height_increase;

        squircle.grounded = true;

      } else if (minOverlap == bottomOverlap) {
        squircle.pos.y = bounds.y - squircle.width;
        squircle.vel.y = -abs(squircle.vel.y);

      } else if (minOverlap == leftOverlap) {
        squircle.pos.x = bounds.x - squircle.width;
        squircle.vel.x = -abs(squircle.vel.x);

      } else if (minOverlap == rightOverlap) {
        squircle.pos.x = bounds.x + bounds.width;
        squircle.vel.x = abs(squircle.vel.x);
      }
    }

    // If sufficiently close to ground, apply ground friction
    bool horizontalOverlap = squircle.pos.x < bounds.x + bounds.width &&
                             bounds.x < squircle.pos.x + squircle.width;
    if (horizontalOverlap &&
        squircle.pos.y <= bounds.y + bounds.height +
                              constants::squircle::ground_friction_height &&
        squircle.pos.y + squircle.width >= bounds.y &&
        squircle.vel.y <= constants::squircle::velocity_deadband) {
      squircle.vel.x = squircle.vel.x -
                       squircle.vel.x *
                           constants::squircle::
                               squircle_ground_friction_percent_decrease_per_s *
                           delta_time;
    }

    // Velocity deadbands
    if (std::abs(squircle.vel.x) < constants::squircle::velocity_deadband) {
      squircle.vel.x = 0;
    }
    if (std::abs(squircle.vel.y) < constants::squircle::velocity_deadband) {
      squircle.vel.y = 0;
    }
  }

  if (squircle.pos.y + squircle.width < game_state->height) {
    struct ChangeViewEvent event {
      "game", "death"
    };

    EventManager::triggerEvent(event);
  }
}

} // namespace physics
