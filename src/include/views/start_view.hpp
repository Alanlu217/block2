#pragma once
#include "view.hpp"

class StartView : public View {
private:
public:
  StartView();

  virtual ~StartView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};