#pragma once

#include "entities/basic_platform.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include "view.hpp"

#include <vector>

enum EditorState { Idle, Selecting, Dragging };

class EditorView : public View {
private:
  GameStateP game_state;

  std::vector<BasicPlatform> *platforms;
  Background *back_ground;

  Camera2D *camera;

  std::vector<BasicPlatform *> selected_platforms;

  EditorState state;

  std::optional<Vector2> mouse_drag_init;

  float gui_rect[4] = {0, 0, 0, 0};
  char file_name[50] = "default";

  void update_selection();

public:
  EditorView(GameStateP state);

  virtual ~EditorView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
