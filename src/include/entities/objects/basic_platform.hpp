#pragma once

#include "entities/objects/object.hpp"

#include <raylib.h>
#include <string_view>

class BasicPlatform : public Object {
public:
  Rectangle rect;

  BasicPlatform();
  BasicPlatform(const BasicPlatform &platform);
  BasicPlatform(float x, float y, float width, float height);

  const std::string id = "Platform";

  void draw() override;

  BoundsP getBounds() override;

  void setPosition(float x, float y) override;

  void update() override;

  const std::string_view getID() override { return id; };

  void load(std::string_view object) override;

  std::string_view save() override;
};
