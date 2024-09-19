#include "entities/objects/ghost_platform.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "window.hpp"

#include <cstdio>
#include <imgui.h>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <string_view>

const std::string GhostPlatform::id = "GhostPlatform";

GhostPlatform::GhostPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

GhostPlatform::GhostPlatform(const GhostPlatform &platform)
    : GhostPlatform(platform.rect.x, platform.rect.y, platform.rect.width,
                    platform.rect.height) {
  on_off_times = platform.on_off_times;
}

GhostPlatform::GhostPlatform() : GhostPlatform(0, 0, 100, 10) {}

void GhostPlatform::init() {
  time_count = 0;
  idx = 0;
  is_on = true;
}

void GhostPlatform::draw() {
  if (is_on) {
    win::drawRectangle(rect, Color{150, 150, 150, 255});
  } else {
    win::drawRectangle(rect, Color{100, 100, 100, 100});
  }
}

ObjectPhysics GhostPlatform::getObjectPhysics() {
  return object_physics = {rect, constants::squircle::bounce_velocity_reduction,
                           constants::squircle::
                               squircle_ground_friction_percent_decrease_per_s,
                           is_on};
}

void GhostPlatform::setPosition(float x, float y) {
  rect.x = x;
  rect.y = y;
}

void GhostPlatform::update(const double dt, GameStateP state) {
  time_count += dt;

  if (time_count > on_off_times[idx]) {
    time_count = 0;
    idx++;

    if (idx > on_off_times.size()) {
      idx = 0;
    }

    is_on = idx % 2 == 0;
  }
}

void GhostPlatform::load(std::string_view object) {
  on_off_times.clear();

  std::stringstream s{std::string{object}};

  float x, y, width, height;

  s >> x >> y >> width >> height;

  int num_times;
  s >> num_times;

  for (int i = 0; i < num_times; i++) {
    double temp;
    s >> temp;

    on_off_times.push_back(temp);
  }

  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
}

std::string GhostPlatform::save() {
  std::stringstream s;
  s << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;

  s << " " << on_off_times.size();

  for (const auto &i : on_off_times) {
    s << " " << i;
  }

  return s.str();
}

void GhostPlatform::showEditorOptions() {
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

  ImGui::SetNextWindowPos(ImVec2{0, 60});
  ImGui::SetNextWindowSize({140, 200});
  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoFocusOnAppearing |
               ImGuiWindowFlags_NoTitleBar;
  ImGui::Begin("Times Editor", NULL, flags);
  int count = 0;
  int to_del = -1;
  for (auto &i : on_off_times) {
    std::string name = "##edit";
    name.append(std::to_string(count));
    std::string name2 = "Del##edi";
    name2.append(std::to_string(count));

    ImGui::SetNextItemWidth(10);
    if (count % 2 == 0) {
      ImGui::Text("T:");
    } else {
      ImGui::Text("F:");
    }
    ImGui::SameLine();

    float temp = i;
    ImGui::SetNextItemWidth(60);
    ImGui::DragFloat(name.c_str(), &temp);
    on_off_times[count] = temp;

    if (on_off_times[count] < 0) {
      on_off_times[count] = 0;
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(20);
    if (ImGui::Button(name2.c_str())) {
      to_del = count;
    }

    count++;
  }

  if (ImGui::Button("Add New")) {
    on_off_times.push_back(1);
  }
  ImGui::End();

  if (to_del != -1) {
    on_off_times.erase(std::next(on_off_times.begin(), to_del));
  }
}
