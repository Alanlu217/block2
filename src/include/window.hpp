#pragma once

#include "managers/resource_manager.hpp"
#include "raylib.h"

namespace win {

void drawRectangle(Rectangle rect, Color color);

void drawTexture(TextureP texture, Vector2 pos, Color color = WHITE);

Vector2 toGameUnits(Vector2 vec);

} // namespace win
