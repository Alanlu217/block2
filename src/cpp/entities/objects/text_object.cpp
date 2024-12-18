#include "entities/objects/text_object.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <imgui.h>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <string_view>

const std::string TextObject::id = "Text";

TextObject::TextObject(Vector2 pos, std::string text, int size)
    : pos(pos), text(text), size(size) {
  font = ResourceManager::getFont("comfortaa.ttf");
}

TextObject::TextObject() : TextObject({0, 0}, "default", 24) {}

TextObject::TextObject(const TextObject &platform)
    : TextObject(platform.pos, platform.text, platform.size) {}

void TextObject::draw() {
  auto s = MeasureTextEx(*font, text.c_str(), size, 0);
  Vector2 new_pos = {pos.x, static_cast<float>(win::toWindowUnits(pos.y, s.y))};
  DrawTextEx(*font, text.c_str(), new_pos, size, 0,
             Color{static_cast<unsigned char>(col[0] * 255),
                   static_cast<unsigned char>(col[1] * 255),
                   static_cast<unsigned char>(col[2] * 255),
                   static_cast<unsigned char>(col[3] * 255)});
};

ObjectPhysics TextObject::getObjectPhysics() {
  auto s = MeasureTextEx(*font, text.c_str(), size, 0);
  return {{pos.x, pos.y, s.x, s.y}, 0, 0, false};
}

void TextObject::setPosition(float x, float y) {
  pos.x = x;
  pos.y = y;
}

void TextObject::update(const double dt, GameStateP state) {}

void TextObject::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, sz;
  s >> x >> y >> sz >> col[0] >> col[1] >> col[2] >> col[3];

  std::string t = s.str();

  int idx = 0;
  for (char i : t) {
    idx++;
    if (i == '>') {
      break;
    }
  }
  t = t.substr(idx, t.size());

  pos.x = x;
  pos.y = y;
  text = t;
  size = sz;

  int count = 0;
  for (char i : text) {
    if (count >= 100)
      break;

    editor_text[count++] = i;
  }
}

std::string TextObject::save() {
  std::stringstream s;
  s << pos.x << " " << pos.y << " " << size << " " << col[0] << " " << col[1]
    << " " << col[2] << " " << col[3] << " >" << text;

  return s.str();
}

void TextObject::showEditorOptions() {
  ImGui::SetNextItemWidth(70);
  ImGui::DragFloat("x", &pos.x);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(70);
  ImGui::DragFloat("y", &pos.y);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(40);
  ImGui::DragInt("s", &size);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(170);
  ImGui::ColorEdit4("##4", col);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(170);
  ImGui::InputText("##5", editor_text, 100);
  ImGui::SameLine();

  text = editor_text;
  if (text == "") {
    text = "NULL";
  }
}
