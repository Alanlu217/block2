#include "views/start_view.hpp"

#include "constants.hpp"

#include "events/event.hpp"
#include "managers/event_manager.hpp"
#include "managers/resource_manager.hpp"
#include "raylib.h"

StartView::StartView() {
  start_button_rect = {float(constants::window_width) / 2 - 100,
                       float(constants::window_height) / 2 - 40, 200, 80};
}

void StartView::init() {
  start_button_font = ResourceManager::getFont("comfortaa.ttf");
  SetTextureFilter(start_button_font->texture, TEXTURE_FILTER_TRILINEAR);
};

void StartView::update(const double deltaTime) {}

void StartView::render(const double deltaTime) {
  auto mouse_pos = GetMousePosition();

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

  auto size = MeasureTextEx(*start_button_font, "Start", 60, 0);

  DrawTextEx(*start_button_font, "Start",
             {float(constants::window_width) / 2 - size.x / 2,
              float(constants::window_height) / 2 - size.y / 2 + 3},
             60, 0, WHITE);
}

void StartView::close() { start_button_font = nullptr; }
