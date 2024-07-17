#pragma once

#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include "view.hpp"

class GameView : public View {
private:
  std::vector<BasicPlatform> *platforms;
  Squircle *squircle;

public:
  GameView(GameStateP state);

  virtual ~GameView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};