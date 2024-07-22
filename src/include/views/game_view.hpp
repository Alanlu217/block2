#pragma once

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/object.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include "view.hpp"

class GameView : public View {
private:
  GameStateP game_state;

  Squircle *squircle;
  Dragger *dragger;
  std::vector<ObjectP> *objects;
  Background *back_ground;

  Camera2D *camera;

  bool haxs = false;

public:
  GameView(GameStateP state);

  virtual ~GameView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
