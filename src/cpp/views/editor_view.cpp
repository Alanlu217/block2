#include "views/editor_view.hpp"

#include "constants.hpp"
#include "entities/objects/basic_platform.hpp"
#include "events/change_view_event.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/physics_manager.hpp"
#include "managers/save_manager.hpp"
#include "window.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <imgui.h>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>

EditorView::EditorView(GameStateP state)
    : game_state(state), objects(&state->objects), camera(&state->game_camera),
      back_ground(&state->entities.back_ground) {}

void EditorView::init() {
  *camera = Camera2D{};
  camera->target = {0, 0};
  camera->offset = {0, 0};
  camera->rotation = 0;
  camera->zoom = 1;

  game_state->height = 0;

  state = Idle;
  selected_objects = {};
  mouse_drag_init = {};
};

void EditorView::update(const double deltaTime) {

  // Update height
  float movement = 0;
  if (float mouse_wheel = GetMouseWheelMove(); mouse_wheel != 0) {
    movement = std::copysign(std::abs(mouse_wheel), mouse_wheel);
  } else {
    if (IsKeyDown(KEY_UP)) {
      movement += 1;
    } else if (IsKeyDown(KEY_DOWN)) {
      movement -= 1;
    }
  }

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    game_state->height += movement * deltaTime * 4000;
  } else {
    game_state->height += movement * deltaTime * 1000;
  }
}

void EditorView::update_selection() {
  auto mouse_pos = win::getMousePos();
  mouse_pos.y += game_state->height;

  // Check for selection
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    bool selected_nothing = true;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{0, 0, 600, 35})) {
      return; // Mouse is in menu
    }

    for (auto &object : *objects) {
      if (CheckCollisionPointRec(mouse_pos, *object->getBounds())) { //
        selected_nothing = false;

        if (std::ranges::find(selected_objects, object.get()) !=
            selected_objects.end()) {      // object already selected
          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Remove object
            selected_objects.erase(std::remove(selected_objects.begin(),
                                               selected_objects.end(),
                                               object.get()),
                                   selected_objects.end());

            state = Idle;

            return;
          } else { // Start Drag
            state = Dragging;
            mouse_drag_init = mouse_pos;

            return;
          }
        } else // object not selected

          if (IsKeyDown(KEY_LEFT_SHIFT)) { // Add object to selection
            selected_objects.push_back(object.get());

            state = Idle;

            return;
          } else { // Set selection to object
            selected_objects.clear();
            selected_objects.push_back(object.get());

            mouse_drag_init = mouse_pos;

            state = Dragging;

            return;
          }
      }
    }

    if (selected_nothing) {
      state = Selecting;
      mouse_drag_init = mouse_pos;

      return;
    }

  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

    if (state == Dragging) {
      state = Idle;
      mouse_drag_init = {};
    }

    if (mouse_pos.x == mouse_drag_init->x && mouse_pos.y == mouse_pos.y &&
        state == Selecting) { // Selection is a point
      selected_objects.clear();
      state = Idle;
      mouse_drag_init = {};

      return;
    }

    if (state == Selecting) {
      const Rectangle selection_rect = {
          std::min(mouse_pos.x, mouse_drag_init->x),
          std::min(mouse_pos.y, mouse_drag_init->y),
          std::abs(mouse_pos.x - mouse_drag_init->x),
          std::abs(mouse_pos.y - mouse_drag_init->y)};

      bool selected_nothing = true;

      for (auto &object : *objects) {
        if (CheckCollisionRecs(selection_rect, *object->getBounds())) {
          selected_nothing = false;

          if (std::ranges::find(selected_objects, object.get()) !=
              selected_objects.end()) {
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
              selected_objects.erase(std::remove(selected_objects.begin(),
                                                 selected_objects.end(),
                                                 object.get()),
                                     selected_objects.end());
            }
          } else {
            selected_objects.push_back(object.get());

            if (selected_objects.size() == 1) {
              auto &object = selected_objects[0];
            }
          }
        }
      }

      if (selected_nothing) {
        selected_objects = {};
      } else {
        state = Idle;
      }
    }
    mouse_drag_init = {};
  } else if (state == Dragging) {
    auto delta = Vector2Subtract(mouse_pos, *mouse_drag_init);
    mouse_drag_init = mouse_pos;

    for (auto object : selected_objects) {
      Rectangle *bounds = object->getBounds();

      object->setPosition(bounds->x + delta.x, bounds->y + delta.y);
    }

    if (selected_objects.size() == 1) {
      auto &object = selected_objects[0];

      // gui_rect[0] = object->rect.x;
      // gui_rect[1] = object->rect.y;
      // gui_rect[2] = object->rect.width;
      // gui_rect[3] = object->rect.height;
    }
  }

  // Delete object
  if (IsKeyPressed(KEY_D)) {
    delete_selected_objects();
  }

  // Add new object
  if (IsKeyPressed(KEY_A)) {
    objects->push_back(
        std::make_unique<BasicPlatform>(mouse_pos.x, mouse_pos.y, 100, 10));
  }

  // Copy object dimensions
  if (IsKeyPressed(KEY_C)) {
    copied_object = std::make_unique<BasicPlatform>(
        *dynamic_cast<BasicPlatform *>(selected_objects[0]));
  }

  // Pasts object
  if (IsKeyPressed(KEY_V)) {
    selected_objects.clear();

    if (copied_object.has_value()) {
      objects->push_back(std::make_unique<BasicPlatform>(
          *dynamic_cast<BasicPlatform *>(copied_object->get())));
    }
  }
}

void EditorView::delete_selected_objects() {
  for (auto plat : selected_objects) {
    auto pos =
        std::find_if(objects->begin(), objects->end(),
                     [plat](auto &object) { return plat == object.get(); });
    if (pos != objects->end())
      objects->erase(pos, objects->end());
  }
  selected_objects = {};
}

void EditorView::render(const double deltaTime) {

  update_selection();

  if (game_state->show_debug) {
    ImGui::Begin("EditorView");

    ImGui::DragFloat("Height:", &game_state->height);

    ImGui::Text("State: %d", state);

    ImGui::Text("Num Selected: %zu", selected_objects.size());

    ImGui::End();
  }

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(ImVec2{600, 35});
  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_AlwaysAutoResize |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
               ImGuiWindowFlags_NoFocusOnAppearing |
               ImGuiWindowFlags_NoTitleBar;

  if (selected_objects.size() == 1) {
    ImGui::Begin("object Editor", NULL, flags);

    selected_objects[0]->showEditorOptions();

    ImGui::End();
  } else if (selected_objects.size() > 1) {
    ImGui::Begin("Multi object Editor", NULL, flags);

    ImGui::DragFloat2("X, Y", multi_object_editor_pos);

    ImGui::SameLine();
    ImGui::Text("Set All: ");
    ImGui::SameLine();

    if (ImGui::Button("X")) {
      for (auto object : selected_objects) {
        object->setPosition(multi_object_editor_pos[0], object->getBounds()->y);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Y")) {
      for (auto object : selected_objects) {
        object->setPosition(object->getBounds()->x, multi_object_editor_pos[1]);
      }
    }

    ImGui::End();
  } else {
    ImGui::Begin("Editor", NULL, flags);

    if (ImGui::Button("Play")) {
      struct ChangeViewEvent event;
      event.new_view = "game";
      event.old_view = "editor";

      EventManager::triggerEvent(event);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
      std::strncpy(file_name,
                   SaveManager::loadFromFile(file_name, game_state).c_str(),
                   50);
    }
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
      std::strncpy(file_name,
                   SaveManager::saveToFile(file_name, game_state).c_str(), 50);
    }
    ImGui::SameLine();
    ImGui::InputText("Save Name", file_name, 50);
    ImGui::SameLine();

    ImGui::End();
  }

  camera->target = {0, -static_cast<float>(game_state->height)};

  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &object : *objects) {
    object->draw();
  }

  for (auto object : selected_objects) {
    win::drawRectangleLines(*object->getBounds(), 2, GREEN);
  }

  if (mouse_drag_init.has_value()) {
    win::drawRectangleLines(
        Rectangle{std::min(mouse_drag_init->x, float(win::getMouseX())),
                  std::min(mouse_drag_init->y,
                           float(win::getMouseY() + game_state->height)),
                  std::abs(win::getMouseX() - mouse_drag_init->x),
                  std::abs(win::getMouseY() + game_state->height -
                           mouse_drag_init->y)},
        2, Color{255, 255, 255, 100});
  }

  EndMode2D();
}

void EditorView::close() {}
