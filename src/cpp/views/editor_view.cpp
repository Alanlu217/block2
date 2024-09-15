#include "views/editor_view.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "entities/objects/objects.hpp"
#include "events/start_test_event.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/input_manager.hpp"
#include "managers/save_manager.hpp"
#include "window.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <imgui.h>
#include <memory>
#include <raylib.h>
#include <raymath.h>

EditorView::EditorView(GameStateP state)
    : game_state(state), objects(&state->objects),
      back_ground(&state->entities.back_ground), camera(&state->game_camera) {}

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
    if (Input::isKeyDown(KEY_UP)) {
      movement += 1;
    } else if (Input::isKeyDown(KEY_DOWN)) {
      movement -= 1;
    }
  }

  if (Input::isKeyDown(KEY_LEFT_SHIFT)) {
    game_state->height += movement * deltaTime * 4000;
  } else {
    game_state->height += movement * deltaTime * 1000;
  }
}

void EditorView::update_selection() {
  auto mouse_pos = win::getMousePos();
  mouse_pos.y += game_state->height;

  // Check for selection
  if (Input::isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    bool selected_nothing = true;

    if (CheckCollisionPointRec(
            GetMousePosition(),
            Rectangle{0, 0, editor_menu_size.x, editor_menu_size.y})) {
      return; // Mouse is in menu
    }

    for (auto &object : *objects) {
      Rectangle bounds = object->getObjectPhysics().bounds;

      if (CheckCollisionPointRec(mouse_pos,
                                 bounds)) { //
        selected_nothing = false;

        if (std::ranges::find(selected_objects, object.get()) !=
            selected_objects.end()) {             // object already selected
          if (Input::isKeyDown(KEY_LEFT_SHIFT)) { // Remove object
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

          if (Input::isKeyDown(KEY_LEFT_SHIFT)) { // Add object to selection
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

  } else if (Input::isMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

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
        Rectangle bounds = object->getObjectPhysics().bounds;
        if (CheckCollisionRecs(selection_rect, bounds)) {
          selected_nothing = false;

          if (std::ranges::find(selected_objects, object.get()) !=
              selected_objects.end()) {
            if (Input::isKeyDown(KEY_LEFT_SHIFT)) {
              selected_objects.erase(std::remove(selected_objects.begin(),
                                                 selected_objects.end(),
                                                 object.get()),
                                     selected_objects.end());
            }
          } else {
            selected_objects.push_back(object.get());
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
      Rectangle bounds = object->getObjectPhysics().bounds;

      object->setPosition(bounds.x + delta.x, bounds.y + delta.y);
    }
  }

  if (CheckCollisionPointRec(
          GetMousePosition(),
          Rectangle{0, 0, editor_menu_size.x, editor_menu_size.y})) {
    return; // Mouse is in menu
  }

  // Delete object
  if (Input::isKeyPressed(KEY_D) || Input::isKeyPressed(KEY_BACKSPACE)) {
    delete_selected_objects();
  }

  // Add new object
  if (Input::isKeyPressed(KEY_A)) {
    ObjectP temp = createObject(object_options[active_object]);
    temp->setPosition(mouse_pos.x, mouse_pos.y);
    objects->push_back(std::move(temp));
  }

  // Copy object dimensions
  if (Input::isKeyPressed(KEY_C)) {
    if (!selected_objects.empty()) {
      copied_object = copyObject(selected_objects[0]);
    }
  }

  // Pasts object
  if (Input::isKeyPressed(KEY_V)) {
    selected_objects.clear();

    if (copied_object.has_value()) {
      objects->push_back(copyObject(copied_object->get()));
    }
  }
}

void EditorView::delete_selected_objects() {
  for (auto plat : selected_objects) {
    auto pos =
        std::find_if(objects->begin(), objects->end(),
                     [plat](auto &object) { return plat == object.get(); });
    if (pos != objects->end())
      objects->erase(pos, pos + 1);
  }
  selected_objects = {};
}

void EditorView::render(const double deltaTime) {

  if (!test_starting) {
    update_selection();
  }

  if (game_state->show_debug) {
    ImGui::Begin("EditorView");

    ImGui::DragFloat("Height:", &game_state->height);

    ImGui::Text("State: %d", state);

    ImGui::Text("Num Selected: %zu", selected_objects.size());

    ImGui::End();
  }

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(editor_menu_size);
  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_AlwaysAutoResize |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
               ImGuiWindowFlags_NoFocusOnAppearing |
               ImGuiWindowFlags_NoTitleBar;

  if (selected_objects.size() > 1) {
    ImGui::Begin("Multi object Editor", NULL, flags);

    ImGui::DragFloat2("X, Y", multi_object_editor_pos);

    ImGui::SameLine();
    ImGui::Text("Set All: ");
    ImGui::SameLine();

    if (ImGui::Button("X")) {
      for (auto object : selected_objects) {
        Rectangle bounds = object->getObjectPhysics().bounds;
        object->setPosition(multi_object_editor_pos[0], bounds.y);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Y")) {
      for (auto object : selected_objects) {
        Rectangle bounds = object->getObjectPhysics().bounds;
        object->setPosition(bounds.x, multi_object_editor_pos[1]);
      }
    }

    ImGui::End();
  } else {
    ImGui::Begin("Editor", NULL, flags);

    if (ImGui::Button("Play")) {
      struct ChangeViewEvent event {
        "editor", "game"
      };

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
    // ImGui::SetNextItemWidth(180);
    ImGui::InputText("Save Name", file_name, 25);

    if (selected_objects.size() == 1) {
      selected_objects[0]->showEditorOptions();
    } else {
      if (ImGui::Button("Test")) {
        test_starting = true;
      }

      ImGui::SameLine();

      // ImGui::SetNextItemWidth(160);
      if (ImGui::BeginCombo("Type", object_options[active_object], 0)) {
        for (int n = 0; n < IM_ARRAYSIZE(object_options); n++) {
          const bool is_selected = (active_object == n);
          if (ImGui::Selectable(object_options[n], is_selected))
            active_object = n;

          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

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
    Rectangle bounds = object->getObjectPhysics().bounds;
    win::drawRectangleLines(bounds, 2, GREEN);
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

  if (test_starting) {
    auto mouse_pos = win::getMousePos();
    mouse_pos.y += game_state->height;

    game_state->entities.squircle.pos = {
        mouse_pos.x - game_state->entities.squircle.width / 2,
        mouse_pos.y - game_state->entities.squircle.width / 2};
    game_state->entities.squircle.draw();

    if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      test_starting = false;

      struct StartTestEvent event;
      event.old_view = "editor";
      event.height = game_state->height;
      event.squircle_pos = game_state->entities.squircle.pos;

      EventManager::triggerEvent(event);
    }
  }

  EndMode2D();
}

void EditorView::close() {}
