#include "entities/dragger.hpp"

#include "constants.hpp"
#include "imgui.h"
#include "managers/resource_manager.hpp"
#include "raylib.h"
#include "window.hpp"

#include <cmath>
#include <iostream>

Dragger::Dragger() {
  direction_arrow_texture = ResourceManager::getTexture("direction_arrow.png");

  SetTextureFilter(*direction_arrow_texture, TEXTURE_FILTER_TRILINEAR);
}

std::optional<Vector2> Dragger::update() {
  if (drag_init_pos.has_value() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    auto drag = Vector2{drag_init_pos->x - win::getMouseX(),
                        drag_init_pos->y - win::getMouseY()};

    drag_init_pos = {};

    return drag;
  } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    drag_init_pos = win::getMousePos();
  }

  return {};
}

void Dragger::draw(const Squircle &squircle) {
  Vector2 pos = squircle.pos;

  ImGui::Begin("Dragger");

  ImGui::DragFloat("Angle", &temp_angle);

  ImGui::End();

  win::drawTexturePro(
      direction_arrow_texture,
      {0, 0, static_cast<float>(direction_arrow_texture->width),
       static_cast<float>(direction_arrow_texture->height)},
      {pos.x + squircle.width / 2,
       pos.y + squircle.width / 2 -
           static_cast<float>(direction_arrow_texture->height) / 2,
       static_cast<float>(direction_arrow_texture->width),
       static_cast<float>(direction_arrow_texture->height)},
      {-constants::dragger::arrow_offset,
       static_cast<float>(direction_arrow_texture->height) / 2},
      temp_angle);
}
