#pragma once

#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"

#include <raylib.h>
#include <string>
#include <string_view>

class TextObject : public Object {
private:
  FontP font;

  Vector2 pos;
  std::string text = "default";
  float col[4];
  char editor_text[100] = "default";
  int size;

public:
  TextObject(Vector2 pos, std::string text, int size);
  TextObject(const TextObject &platform);
  TextObject();

  static const std::string id;

  void draw() override;

  virtual ObjectPhysics getObjectPhysics() override;

  void setPosition(float x, float y) override;

  void update(GameStateP state) override;

  const std::string_view getID() override { return id; };

  void load(std::string_view object) override;

  std::string save() override;

  void showEditorOptions() override;
};
