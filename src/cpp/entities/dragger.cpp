#include "entities/dragger.hpp"

#include "constants.hpp"
#include "managers/input_manager.hpp"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <cmath>
#include <imgui.h>
#include <raylib.h>

Dragger::Dragger() {
  direction_arrow_texture = ResourceManager::getTexture("direction_arrow.png");

  SetTextureFilter(*direction_arrow_texture, TEXTURE_FILTER_TRILINEAR);
}

std::optional<Vector2> Dragger::update() {
  if (drag_init_pos.has_value() &&
      Input::isMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    float x_diff = win::getMouseX() - drag_init_pos->x;
    float y_diff = win::getMouseY() - drag_init_pos->y;

    float dist = constants::dragger::velocity_multiplier *
                 std::sqrt(std::pow(y_diff, 2) + std::pow(x_diff, 2));
    dist = std::min(dist, constants::dragger::max_vertical_velocity);

    drag_init_pos = {};

    float angle = std::atan2(y_diff, x_diff) + PI;

    Vector2 drag = {dist * std::cos(angle), dist * std::sin(angle)};

    return drag;

  } else if (Input::isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    drag_init_pos = win::getMousePos();
  }

  return {};
}

void Dragger::drawArrow(const Squircle &squircle) {
  Vector2 pos = squircle.pos;

  if (drag_init_pos.has_value()) {
    float x_diff = win::getMouseX() - drag_init_pos->x;
    float y_diff = win::getMouseY() - drag_init_pos->y;

    float dist = std::sqrt(std::pow(y_diff, 2) + std::pow(x_diff, 2));

    float scale =
        std::min(1.0, dist / constants::dragger::max_vertical_velocity + 0.6);

    if (dist < 1) {
      temp_angle = -90;
    } else {
      temp_angle = std::atan2(win::getMouseY() - drag_init_pos->y,
                              drag_init_pos->x - win::getMouseX()) *
                   180 / PI;
    }

    Rectangle source = {0, 0, float(direction_arrow_texture->width),
                        float(direction_arrow_texture->height)};

    Rectangle dest = {pos.x + squircle.width / 2, pos.y + squircle.width / 2,
                      float(direction_arrow_texture->width) * scale,
                      float(direction_arrow_texture->height) * scale};

    Vector2 origin = {-constants::dragger::arrow_offset,
                      (float(direction_arrow_texture->height) * scale) / 2};

    win::drawTexturePro(direction_arrow_texture, source, dest, origin,
                        temp_angle);
  }
}

void Dragger::drawCircle() {
  if (drag_init_pos.has_value()) {
    win::drawCircle(*drag_init_pos, 10, GREEN);
  }
}
