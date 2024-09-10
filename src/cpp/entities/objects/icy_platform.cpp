#include "entities/objects/icy_platform.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "raylib.h"
#include "window.hpp"

#include <iostream>
#include <sstream>

const std::string IcyPlatform::id = "IcyPlatform";

IcyPlatform::IcyPlatform() { rect = {0, 0, 100, 10}; }

IcyPlatform::IcyPlatform(const IcyPlatform &platform) : rect(platform.rect) {}

IcyPlatform::IcyPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

void IcyPlatform::draw() { win::drawRectangle(rect, BLUE); }

ObjectPhysics IcyPlatform::getObjectPhysics() {
  return object_physics = {rect, constants::squircle::bounce_velocity_reduction,
                           0};
}

void IcyPlatform::setPosition(float x, float y) {
  rect.x = x;
  rect.y = y;
}

void IcyPlatform::update(GameStateP state) {}

void IcyPlatform::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, width, height;

  s >> x >> y >> width >> height;

  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
}

std::string IcyPlatform::save() {
  std::stringstream s;
  s << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;

  return s.str();
}

void IcyPlatform::showEditorOptions() {
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