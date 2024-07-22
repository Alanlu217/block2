#pragma once

#include <memory>
#include <raylib.h>
#include <string_view>
#include <vector>

typedef Rectangle *BoundsP;

class Object {
public:
  virtual ~Object() = default;

  virtual void draw() {};

  virtual BoundsP getBounds() = 0;

  virtual void setPosition(float x, float y) = 0;

  virtual void update() {};

  virtual const std::string_view getID() = 0;

  virtual void load(std::string_view object) = 0;

  virtual std::string save() = 0;
};

typedef std::unique_ptr<Object> ObjectP;
