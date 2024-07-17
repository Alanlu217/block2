#pragma once

#include <memory>

class View {
protected:
  ~View() = default;

public:
  virtual void init() = 0;

  virtual void update(const double deltaTime) = 0;

  virtual void render(const double deltaTime) = 0;

  virtual void close() = 0;
};

typedef std::shared_ptr<View> ViewP;