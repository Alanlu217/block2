#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"

#include <raylib.h>
#include <string>
#include <string_view>

class DoubleJumpItem : public Object {
private:
  TextureP icon;
  FontP font;

  Vector2 position;
  int jumps;
  bool used = false;

public:
  DoubleJumpItem(float x, float y, int jumps);
  DoubleJumpItem(const DoubleJumpItem &platform);
  DoubleJumpItem();

  static const std::string id;

  void init() override;

  void draw() override;

  virtual ObjectPhysics getObjectPhysics() override;

  void setPosition(float x, float y) override;

  void update(const double dt, GameStateP state) override;

  const std::string_view getID() override { return id; };

  void load(std::string_view object) override;

  std::string save() override;

  void showEditorOptions() override;
};
