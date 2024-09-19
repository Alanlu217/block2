#include "managers/physics_manager.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "entities/squircle.hpp"
#include "events/change_view_event.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/save_manager.hpp"

#include <algorithm>
#include <raylib.h>

namespace physics {

void update(const double delta_time, GameStateP game_state) {

  // Update height
  // Currently linear, could be logarithmic later
  if (game_state->height_should_increase) {
    game_state->height += delta_time * constants::game::height_increase_per_s;

    game_state->max_height =
        std::max(game_state->max_height, game_state->height);
  }

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

  // Reset squircle grounded
  squircle.grounded = false;

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
  for (ObjectP &object : game_state->objects) {
    object->update(delta_time, game_state);

    ObjectPhysics physics = object->getObjectPhysics();
    if (!physics.isCollider) {
      continue;
    }

    Rectangle bounds = physics.bounds;

    // Collision detected
    if (CheckCollisionRecs(bounds, squircle.getBounds())) {
      int leftOverlap = squircle.pos.x + squircle.width - bounds.x;
      int rightOverlap = bounds.x + bounds.width - squircle.pos.x;
      int topOverlap = bounds.y + bounds.height - squircle.pos.y;
      int bottomOverlap = squircle.pos.y + squircle.width - bounds.y;

      // Find the direction with minimum overlap distance
      int minOverlap =
          std::min({leftOverlap, rightOverlap, topOverlap, bottomOverlap});

      if (minOverlap == topOverlap) {
        squircle.pos.y = bounds.y + bounds.height;
        squircle.vel.y =
            abs(squircle.vel.y * physics.bounce_velocity_reduction);

        // If is idle, adjust height
        game_state->height +=
            std::max((squircle.pos.y - game_state->height) - 300, 0.0f) *
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
      squircle.vel.x = squircle.vel.x - squircle.vel.x *
                                            physics.ground_friction_reduction *
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
    SaveManager::saveScoreToFile(game_state->name, game_state);
    game_state->death_message = "Died from the floor";

    struct ChangeViewEvent event {
      "game", "death"
    };

    EventManager::triggerEvent(event);
  }
}

} // namespace physics
