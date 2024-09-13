#pragma once

#include "game_state.hpp"
#include "managers/resource_manager.hpp"
#include "view.hpp"

class StartView : public View {
private:
  GameStateP game_state;

  Rectangle start_button_rect;
  Rectangle editor_button_rect;

  FontP button_font;

  char file_name[50] = "default";

public:
  StartView(GameStateP state);

  virtual ~StartView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
