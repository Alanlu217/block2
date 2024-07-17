#include <string>

namespace constants {

const std::string window_title = "block";

const int window_width = 600;
const int window_height = 800;

namespace squircle {

  const float gravity_acceleration = 1600;
  const float air_friction_percent_decrease_per_s = 1;
  const float squircle_ground_friction_percent_decrease_per_s = 10;
  const float ground_friction_height = 10;
  const float bounce_velocity_reduction = 0.2;
  const float velocity_deadband = 0.2;

}

namespace dragger {

  const float velocity_multiplier = 4;
  const float max_vertical_velocity = 1200;
  const float max_horizontal_velocity = 800;

  const float arrow_offset = 50;

}

namespace game {

  const float height_increase_per_s = 10;

}

}
