#pragma once

#include "managers/resource_manager.hpp"
#include "raylib.h"
#include "view.hpp"

class StartView : public View {
private:
  Rectangle start_button_rect;
  Rectangle editor_button_rect;

  FontP button_font;

public:
  StartView();

  virtual ~StartView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
