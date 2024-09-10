#pragma once

#include <memory>
#include <raylib.h>
#include <string_view>

struct GameState;

struct ObjectPhysics {
  Rectangle bounds;

  // Percentage decrease per bounce
  double bounce_velocity_reduction;

  // Percentage decrease per second
  double ground_friction_reduction;
};

class Object {
public:
  virtual ~Object() = default;

  virtual void draw() {};

  virtual ObjectPhysics getObjectPhysics() = 0;

  virtual void setPosition(float x, float y) = 0;

  virtual void update(std::shared_ptr<GameState> state) {};

  virtual const std::string_view getID() = 0;

  virtual void load(std::string_view object) = 0;

  virtual std::string save() = 0;

  virtual void showEditorOptions() {};
};

typedef std::unique_ptr<Object> ObjectP;