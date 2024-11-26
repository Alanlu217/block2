#pragma once

#include "entities/objects/object.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "view.hpp"

#include <vector>

enum EditorState { Idle, Selecting, Dragging };

class EditorView : public View {
private:
  GameStateP game_state;

  ImVec2 editor_menu_size = {600, 60};

  std::vector<ObjectP> *objects;
  Background *back_ground;

  Camera2D *camera;

  std::vector<Object *> selected_objects;
  std::optional<ObjectP> copied_object;

  EditorState state;

  std::optional<Vector2> mouse_drag_init;
  bool test_starting = false;

  float multi_object_editor_pos[2] = {0, 0};
  char file_name[50] = "default";

  int active_object = 0;

  void update_selection();

  void delete_selected_objects();

public:
  EditorView(GameStateP state);

  virtual ~EditorView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
