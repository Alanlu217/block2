#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"

#include <raylib.h>
#include <string>
#include <string_view>

class GhostPlatform : public Object {
private:
  Rectangle rect;

  double offset_time = 0;
  std::vector<double> on_off_times = {1, 1};
  double time_count = 0;
  int idx = 0;
  bool is_on = true;
  bool offset_done = false;

public:
  GhostPlatform(float x, float y, float width, float height);
  GhostPlatform(const GhostPlatform &platform);
  GhostPlatform();

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
