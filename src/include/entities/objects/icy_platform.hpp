#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"

#include <raylib.h>
#include <string>
#include <string_view>

class IcyPlatform : public Object {
private:
  Rectangle rect;

public:
  IcyPlatform(float x, float y, float width, float height);
  IcyPlatform(const IcyPlatform &platform);
  IcyPlatform();

  static const std::string id;

  void draw() override;

  virtual ObjectPhysics getObjectPhysics() override;

  void setPosition(float x, float y) override;

  void update(const double dt, GameStateP state) override;

  const std::string_view getID() override { return id; };

  void load(std::string_view object) override;

  std::string save() override;

  void showEditorOptions() override;
};
