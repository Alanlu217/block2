#include "entities/objects/double_jump_item.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <imgui.h>
#include <raylib.h>
#include <sstream>
#include <string_view>

const std::string DoubleJumpItem::id = "DoubleJump";

DoubleJumpItem::DoubleJumpItem(float x, float y, int jumps) : jumps(jumps) {
  position.x = x;
  position.y = y;

  icon = ResourceManager::getTexture("double_jump_icon.png");
  font = ResourceManager::getFont("comfortaa.ttf");
}

DoubleJumpItem::DoubleJumpItem(const DoubleJumpItem &platform)
    : DoubleJumpItem(platform.position.x, platform.position.y, platform.jumps) {
}

DoubleJumpItem::DoubleJumpItem() : DoubleJumpItem(0, 0, 1) {}

void DoubleJumpItem::init() { used = false; }

void DoubleJumpItem::draw() {
  if (!used) {
    win::drawTexturePro(icon,
                        {0, 0, static_cast<float>(icon->width),
                         static_cast<float>(icon->height)},
                        {position.x, position.y - 40, 40, 40}, {0, 0}, 0,
                        WHITE);

    DrawTextEx(*font, std::to_string(jumps).c_str(),
               {position.x + 40,
                static_cast<float>(win::toWindowUnits(position.y, 0)) - 40},
               12, 0, WHITE);
  }
}

ObjectPhysics DoubleJumpItem::getObjectPhysics() {
  return {{position.x, position.y, 40, 40},
          constants::squircle::bounce_velocity_reduction,
          constants::squircle::squircle_ground_friction_percent_decrease_per_s,
          false};
}

void DoubleJumpItem::setPosition(float x, float y) {
  position.x = x;
  position.y = y;
}

void DoubleJumpItem::update(const double dt, GameStateP state) {
  if (used) {
    return;
  }

  if (CheckCollisionRecs(state->entities.squircle.getBounds(),
                         {position.x, position.y, 40, 40})) {
    state->double_jumps += jumps;

    used = true;
  }
}

void DoubleJumpItem::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y;
  int j;

  s >> x >> y >> j;

  position.x = x;
  position.y = y;
  jumps = j;
}

std::string DoubleJumpItem::save() {
  std::stringstream s;
  s << position.x << " " << position.y << " " << jumps;

  return s.str();
}

void DoubleJumpItem::showEditorOptions() {
  ImGui::PushItemWidth(100);
  ImGui::DragFloat("##1", &position.x);
  ImGui::SameLine();
  ImGui::DragFloat("##2", &position.y);
  ImGui::SameLine();
  ImGui::DragInt("##3", &jumps);
  ImGui::SameLine();
  ImGui::Text("X Y Jumps");

  if (jumps < 1) {
    jumps = 1;
  }
}
