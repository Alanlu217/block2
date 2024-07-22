#include "views/death_view.hpp"

#include "constants.hpp"
#include "events/event.hpp"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"

#include <raylib.h>

DeathView::DeathView() {
  exit_button_rect = {float(constants::window_width) / 2 - 120,
                      float(constants::window_height) / 2 + 40, 240, 80};

  restart_button_rect = {float(constants::window_width) / 2 - 120,
                         float(constants::window_height) / 2 - 120, 240, 80};
}

void DeathView::init() {
  button_font = ResourceManager::getFont("comfortaa.ttf");
  SetTextureFilter(button_font->texture, TEXTURE_FILTER_TRILINEAR);
};

void DeathView::update(const double deltaTime) {}

void DeathView::render(const double deltaTime) {
  auto mouse_pos = GetMousePosition();

  // Start Button
  if (CheckCollisionPointRec(mouse_pos, exit_button_rect)) {
    DrawRectangleRec(exit_button_rect, Color{255, 255, 255, 50});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      struct ChangeViewEvent event {
        "death", "start"
      };
      EventManager::triggerEvent(event);
    }
  } else {
    DrawRectangleRec(exit_button_rect, Color{255, 255, 255, 30});
  }
  DrawRectangleLinesEx(exit_button_rect, 2, Color{255, 255, 255, 50});

  auto size = MeasureTextEx(*button_font, "Exit", 60, 0);

  DrawTextEx(*button_font, "Exit",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 + 83},
             60, 0, WHITE);

  // Editor Button
  if (CheckCollisionPointRec(mouse_pos, restart_button_rect)) {
    DrawRectangleRec(restart_button_rect, Color{255, 255, 255, 50});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      struct ChangeViewEvent event {
        "death", "game"
      };
      EventManager::triggerEvent(event);
    }
  } else {
    DrawRectangleRec(restart_button_rect, Color{255, 255, 255, 30});
  }
  DrawRectangleLinesEx(restart_button_rect, 2, Color{255, 255, 255, 50});

  size = MeasureTextEx(*button_font, "Restart", 60, 0);
  DrawTextEx(*button_font, "Restart",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 - 77},
             60, 0, WHITE);
}

void DeathView::close() { button_font = nullptr; }
