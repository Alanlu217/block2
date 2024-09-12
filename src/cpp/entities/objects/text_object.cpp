#include "entities/objects/text_object.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <raylib.h>
#include <sstream>
#include <string_view>

const std::string TextObject::id = "Text";

TextObject::TextObject() {
  pos = {0, 0};
  text = "default";
  size = 12;

  font = ResourceManager::getFont("comfortaa.ttf");
}
TextObject::TextObject(const TextObject &platform)
    : pos(platform.pos), text(platform.text), size(platform.size) {
  font = ResourceManager::getFont("comfortaa.ttf");
}
TextObject::TextObject(Vector2 pos, std::string text, int size)
    : pos(pos), text(text), size(size) {
  font = ResourceManager::getFont("comfortaa.ttf");
}

void TextObject::draw() {
  auto s = MeasureTextEx(*font, text.c_str(), size, 0);
  Vector2 new_pos = {pos.x, static_cast<float>(win::toWindowUnits(pos.y, s.y))};
  DrawTextEx(*font, text.c_str(), new_pos, size, 0, WHITE);
};

ObjectPhysics TextObject::getObjectPhysics() {
  auto s = MeasureTextEx(*font, text.c_str(), size, 0);
  return object_physics = {{pos.x, pos.y, s.x, s.y}, 0, 0, false};
}

void TextObject::setPosition(float x, float y) {
  pos.x = x;
  pos.y = y;
}

void TextObject::update(GameStateP state) {}

void TextObject::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, sz;
  s >> x >> y >> sz;

  std::string t;
  s >> t;

  pos.x = x;
  pos.y = y;
  text = t;
  size = sz;
}

std::string TextObject::save() {
  std::stringstream s;
  s << pos.x << " " << pos.y << " " << size << " " << text;

  return s.str();
}

void TextObject::showEditorOptions() {
  ImGui::PushItemWidth(100);
  ImGui::DragFloat("##1", &pos.x);
  ImGui::SameLine();
  ImGui::DragFloat("##2", &pos.y);
  ImGui::SameLine();
  ImGui::DragInt("##3", &size);
  ImGui::SameLine();
  ImGui::InputText("##4", editor_text, 100);
  ImGui::SameLine();
  ImGui::Text("X Y W H");

  text = editor_text;
  if (text == "") {
    text = "NULL";
  }
}
