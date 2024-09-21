#pragma once

#include "entities/objects/object.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include "managers/resource_manager.hpp"
#include "view.hpp"

class GameView : public View {
private:
  GameStateP game_state;

  Squircle *squircle;
  Dragger *dragger;
  std::vector<ObjectP> *objects;
  Background *back_ground;

  TextureP play_button;
  FontP button_font;

  Camera2D *camera;

  bool paused = false;

  bool haxs = false;

  void displayDebug();

public:
  GameView(GameStateP state);

  virtual ~GameView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
