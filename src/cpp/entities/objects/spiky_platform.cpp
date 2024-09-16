#include "entities/objects/spiky_platform.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/save_manager.hpp"
#include "window.hpp"

#include <imgui.h>
#include <iostream>
#include <raylib.h>
#include <sstream>

const std::string SpikyPlatform::id = "SpikyPlatform";

SpikyPlatform::SpikyPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}
SpikyPlatform::SpikyPlatform() : SpikyPlatform(0, 0, 100, 10) {}

SpikyPlatform::SpikyPlatform(const SpikyPlatform &platform)
    : SpikyPlatform(platform.rect.x, platform.rect.y, platform.rect.width,
                    platform.rect.height) {}

void SpikyPlatform::draw() { win::drawRectangle(rect, RED); }

ObjectPhysics SpikyPlatform::getObjectPhysics() {
  return {rect, constants::squircle::bounce_velocity_reduction,
          constants::squircle::squircle_ground_friction_percent_decrease_per_s,
          true};
};

void SpikyPlatform::setPosition(float x, float y) {
  rect.x = x;
  rect.y = y;
}

void SpikyPlatform::update(GameStateP state) {
  if (CheckCollisionRecs(state->entities.squircle.getBounds(), rect)) {
    SaveManager::saveScoreToFile(state->name, state);
    state->death_message = "Died from a spiky platform";

    struct ChangeViewEvent event {
      "game", "death"
    };

    EventManager::triggerEvent(event);
  }
}

void SpikyPlatform::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, width, height;

  s >> x >> y >> width >> height;

  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
}

std::string SpikyPlatform::save() {
  std::stringstream s;
  s << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;

  return s.str();
}

void SpikyPlatform::showEditorOptions() {
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
