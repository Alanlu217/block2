#include "views/start_view.hpp"

#include "constants.hpp"
#include "events/event.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/save_manager.hpp"

#include <cstdio>
#include <cstring>
#include <raylib.h>

StartView::StartView(GameStateP state) : game_state(state) {
  start_button_rect = {float(constants::window_width) / 2 - 120,
                       float(constants::window_height) / 2 - 120, 240, 80};

  editor_button_rect = {float(constants::window_width) / 2 - 120,
                        float(constants::window_height) / 2 + 40, 240, 80};
}

void StartView::init() {
  button_font = ResourceManager::getFont("comfortaa.ttf");
  SetTextureFilter(button_font->texture, TEXTURE_FILTER_TRILINEAR);
};

void StartView::update(const double deltaTime) {}

void StartView::render(const double deltaTime) {
  auto mouse_pos = GetMousePosition();

  // Start Button
  if (CheckCollisionPointRec(mouse_pos, start_button_rect)) {
    DrawRectangleRec(start_button_rect, Color{255, 255, 255, 50});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      struct ChangeViewEvent event {
        "start", "game"
      };
      EventManager::triggerEvent(event);
    }
  } else {
    DrawRectangleRec(start_button_rect, Color{255, 255, 255, 30});
  }
  DrawRectangleLinesEx(start_button_rect, 2, Color{255, 255, 255, 50});

  auto size = MeasureTextEx(*button_font, "Start", 60, 0);

  DrawTextEx(*button_font, "Start",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 - 77},
             60, 0, WHITE);

  // Editor Button
  if (CheckCollisionPointRec(mouse_pos, editor_button_rect)) {
    DrawRectangleRec(editor_button_rect, Color{255, 255, 255, 50});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      struct ChangeViewEvent event {
        "start", "editor"
      };
      EventManager::triggerEvent(event);
    }
  } else {
    DrawRectangleRec(editor_button_rect, Color{255, 255, 255, 30});
  }
  DrawRectangleLinesEx(editor_button_rect, 2, Color{255, 255, 255, 50});

  size = MeasureTextEx(*button_font, "Editor", 60, 0);
  DrawTextEx(*button_font, "Editor",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 + 83},
             60, 0, WHITE);

  char version[10];
  std::snprintf(version, 10, "%s.%s.%s", VERSION_MAJOR, VERSION_MINOR,
                VERSION_PATCH);

  size = MeasureTextEx(*button_font, version, 14, 0);
  DrawTextEx(*button_font, version,
             {5, float(constants::window_height) - size.y - 5}, 14, 0, WHITE);

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(ImVec2{600, 35});
  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_AlwaysAutoResize |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
               ImGuiWindowFlags_NoFocusOnAppearing |
               ImGuiWindowFlags_NoTitleBar;

  ImGui::Begin("Level Selector", NULL, flags);
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    std::strncpy(file_name,
                 SaveManager::loadFromFile(file_name, game_state).c_str(), 50);
  }
  ImGui::SameLine();
  ImGui::InputText("Save Name", file_name, 25);
  ImGui::End();

  if (IsFileDropped()) {
    FilePathList files = LoadDroppedFiles();
    std::string file{files.paths[0]};
    UnloadDroppedFiles(files);
    std::strncpy(file_name,
                 SaveManager::loadFromExternalFile(file, game_state).c_str(),
                 50);
  }
}

void StartView::close() { button_font = nullptr; }
