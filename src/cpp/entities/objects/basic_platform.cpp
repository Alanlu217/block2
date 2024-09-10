#include "entities/objects/basic_platform.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "window.hpp"

#include <raylib.h>
#include <sstream>
#include <string_view>

const std::string BasicPlatform::id = "Platform";

BasicPlatform::BasicPlatform() { rect = {0, 0, 100, 10}; }
BasicPlatform::BasicPlatform(const BasicPlatform &platform)
    : rect(platform.rect) {}
BasicPlatform::BasicPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

void BasicPlatform::draw() { win::drawRectangle(rect, WHITE); }

ObjectPhysics BasicPlatform::getObjectPhysics() {
  return object_physics = {rect, constants::squircle::bounce_velocity_reduction,
                           constants::squircle::
                               squircle_ground_friction_percent_decrease_per_s};
}

void BasicPlatform::setPosition(float x, float y) {
  rect.x = x;
  rect.y = y;
}

void BasicPlatform::update(GameStateP state) {}

void BasicPlatform::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, width, height;

  s >> x >> y >> width >> height;

  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
}

std::string BasicPlatform::save() {
  std::stringstream s;
  s << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;

  return s.str();
}

void BasicPlatform::showEditorOptions() {
  ImGui::PushItemWidth(100);
  ImGui::DragFloat("##1", &rect.x);
  ImGui::SameLine();
  ImGui::DragFloat("##2", &rect.y);
  ImGui::SameLine();
  ImGui::DragFloat("##3", &rect.width);
  ImGui::SameLine();
  ImGui::DragFloat("##4", &rect.height);
  ImGui::SameLine();
  ImGui::Text("X Y W H");
}