#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"

#include <raylib.h>
#include <string>
#include <string_view>

class SpikyPlatform : public Object {
private:
  Rectangle rect;

public:
  SpikyPlatform();
  SpikyPlatform(const SpikyPlatform &platform);
  SpikyPlatform(float x, float y, float width, float height);

  static const std::string id;

  void draw() override;

  BoundsP getBounds() override;

  void setPosition(float x, float y) override;

  void update(GameStateP state) override;

  const std::string_view getID() override { return id; };

  void load(std::string_view object) override;

  std::string save() override;

  void showEditorOptions() override;
};
