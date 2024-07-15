#pragma once

#include "raylib-cpp.hpp"

class View {
protected:
  ~View() = default;

public:
  virtual void init() = 0;

  virtual void update(raylib::Window &window, const double deltaTime) = 0;

  virtual void render(raylib::Window &window, const double deltaTime) = 0;

  virtual void close() = 0;
};

typedef std::shared_ptr<View> ViewP;