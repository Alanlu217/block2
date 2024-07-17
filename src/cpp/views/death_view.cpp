#include "views/death_view.hpp"

#include "constants.hpp"

#include "events/event.hpp"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "raylib.h"

DeathView::DeathView() {
  restart_button_rect = {float(constants::window_width) / 2 - 120,
                         float(constants::window_height) / 2 - 40, 240, 80};
}

void DeathView::init() {
  restart_button_font = ResourceManager::getFont("comfortaa.ttf");
  SetTextureFilter(restart_button_font->texture, TEXTURE_FILTER_TRILINEAR);
};

void DeathView::update(const double deltaTime) {}

void DeathView::render(const double deltaTime) {
  auto mouse_pos = GetMousePosition();

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

  auto size = MeasureTextEx(*restart_button_font, "Restart", 60, 0);

  DrawTextEx(*restart_button_font, "Restart",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 + 3},
             60, 0, WHITE);
}

void DeathView::close() { restart_button_font = nullptr; }
