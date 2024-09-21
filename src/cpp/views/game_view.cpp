#include "views/game_view.hpp"

#include "constants.hpp"
#include "game_state.hpp"
#include "managers/event_manager.hpp"
#include "managers/input_manager.hpp"
#include "managers/physics_manager.hpp"
#include "managers/resource_manager.hpp"

#include <imgui.h>
#include <raylib.h>

GameView::GameView(GameStateP state)
    : game_state(state), squircle(&state->entities.squircle),
      dragger(&state->entities.dragger), objects(&state->objects),
      back_ground(&state->entities.back_ground), camera(&state->game_camera) {}

void GameView::init() {
  squircle->pos = {300.0f - float(squircle->width) / 2, 100};
  squircle->vel = {0, 0};

  *camera = Camera2D{};
  camera->target = {0, 0};
  camera->offset = {0, 0};
  camera->rotation = 0;
  camera->zoom = 1;

  game_state->height = 0;
  game_state->jumps_made = 0;

  play_button = ResourceManager::getTexture("play.png");
  button_font = ResourceManager::getFont("comfortaa.ttf");

  paused = false;

  for (auto &object : game_state->objects) {
    object->init();
  }
};

void GameView::update(const double deltaTime) {
  if (paused) {
    return;
  }

  if (auto drag = dragger->update(); drag.has_value()) {
    TraceLog(LOG_INFO, "Drag at: X: %f, Y: %f", drag->x, drag->y);
    if (squircle->grounded || haxs) {
      squircle->vel = *drag;
      game_state->jumps_made++;
    }
  }

  physics::update(deltaTime, game_state);
}

void GameView::displayDebug() {
  ImGui::Begin("GameView");

  ImGui::DragFloat("Height:", &game_state->height);
  ImGui::DragFloat("Max Height:", &game_state->max_height);

  ImGui::Text("Squircle:\nPos: %f, %f\nVel: %f, %f\nGrounded: %d",
              squircle->pos.x, squircle->pos.y, squircle->vel.x,
              squircle->vel.y, squircle->grounded);

  if (ImGui::Button("Haxs")) {
    haxs = !haxs;
  }
  ImGui::SameLine();
  ImGui::Text(": %d", haxs);
  if (ImGui::Button("Height Haxs")) {
    game_state->height_should_increase = !game_state->height_should_increase;
  }
  ImGui::SameLine();

  ImGui::Text(": %d", game_state->height_should_increase);

  ImGui::End();
}

void GameView::render(const double deltaTime) {
  if (game_state->show_debug) {
    displayDebug();
  }

  // Move camera to current height
  camera->target = {0, -static_cast<float>(game_state->height)};

  // Begin camera and render game elements
  BeginMode2D(*camera);

  back_ground->update(game_state->height);
  back_ground->draw();

  for (auto &platform : *objects) {
    platform->draw();
  }

  squircle->draw();
  dragger->drawArrow(*squircle);
  EndMode2D();

  game_state->entities.height_display.draw(
      game_state->height, game_state->max_height, game_state->jumps_made);

  dragger->drawCircle();

  unsigned char opacity = 30;
  if (CheckCollisionPointRec(GetMousePosition(), {530, 10, 60, 60})) {
    if (Input::isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      paused = !paused;
      dragger->reset();
    }
    opacity = 50;
  }

  DrawRectangle(530, 10, 60, 60, Color{255, 255, 255, opacity});
  DrawRectangleLinesEx({530, 10, 60, 60}, 2, Color{255, 255, 255, 50});
  if (paused) {
    DrawRectangle(0, 0, constants::window_width, constants::window_height,
                  Color{255, 255, 255, 50});
    DrawTexturePro(*play_button,
                   {0, 0, static_cast<float>(play_button->width),
                    static_cast<float>(play_button->height)},
                   {535, 20, 50, 40}, {0, 0}, 0, WHITE);

    Rectangle exit_button_rect = {constants::window_width / 2 - 60,
                                  constants::window_height / 2 - 30, 120, 60};
    if (CheckCollisionPointRec(GetMousePosition(), exit_button_rect)) {
      DrawRectangleRec(exit_button_rect, Color{255, 255, 255, 50});

      if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        struct ChangeViewEvent event {
          "game", "start"
        };
        EventManager::triggerEvent(event);
      }
    } else {
      DrawRectangleRec(exit_button_rect, Color{255, 255, 255, 30});
    }
    DrawRectangleLinesEx(exit_button_rect, 2, Color{255, 255, 255, 50});

    auto size = MeasureTextEx(*button_font, "Exit", 50, 0);

    DrawTextEx(*button_font, "Exit",
               {float(constants::window_width) / 2 - size.x / 2,
                float(constants::window_height) / 2 - size.y / 2 + 2},
               50, 0, WHITE);
  } else {
    DrawRectangleRounded({546, 20, 8, 40}, 1, 10, WHITE);
    DrawRectangleRounded({566, 20, 8, 40}, 1, 10, WHITE);
  }
}

void GameView::close() {
  play_button.reset();
  button_font.reset();
}
