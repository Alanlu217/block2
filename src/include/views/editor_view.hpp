#pragma once

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/object.hpp"
#include "entities/squircle.hpp"
#include "game_state.hpp"
#include "view.hpp"

#include <vector>

enum EditorState { Idle, Selecting, Dragging };

class EditorView : public View {
private:
  GameStateP game_state;

  std::vector<ObjectP> *objects;
  Background *back_ground;

  Camera2D *camera;

  std::vector<Object *> selected_objects;
  std::optional<ObjectP> copied_object;

  EditorState state;

  std::optional<Vector2> mouse_drag_init;

  float multi_object_editor_pos[2] = {0, 0};
  char file_name[50] = "default";

  int active_object = 0;

  const char *object_options[3] = {"basic_platform", "spiky_platform",
                                   "icy_platform"};

  void update_selection();

  void delete_selected_objects();

  ObjectP createObject();

public:
  EditorView(GameStateP state);

  virtual ~EditorView() = default;

  virtual void init() override;

  virtual void update(const double deltaTime) override;

  virtual void render(const double deltaTime) override;

  virtual void close() override;
};
