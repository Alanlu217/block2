#pragma once

#include "managers/resource_manager.hpp"
#include "raylib.h"
#include "view.hpp"

class DeathView : public View {
private:
  Rectangle restart_button_rect;

  FontP restart_button_font;

public:
  DeathView();

  virtual ~DeathView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
