#pragma once

#include "game_state.hpp"
#include "managers/resource_manager.hpp"
#include "view.hpp"

class DeathView : public View {
private:
  GameStateP game_state;

  Rectangle restart_button_rect;
  Rectangle exit_button_rect;

  FontP button_font;

public:
  DeathView(GameStateP state);

  virtual ~DeathView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
