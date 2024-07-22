#pragma once

#include "managers/resource_manager.hpp"

#include <raylib.h>

namespace win {

double toWindowUnits(double y, double height);

void drawRectangle(Rectangle rect, Color color);

void drawRectangleLines(Rectangle rect, float width, Color color);

void drawCircle(Vector2 pos, float radius, Color color);

void drawTexture(TextureP texture, Vector2 pos, float rotation = 0,
                 float scale = 1, Color color = WHITE);

void drawTexturePro(TextureP texture, Rectangle source, Rectangle dest,
                    Vector2 origin, float rotation = 0, Color color = WHITE);

Vector2 toGameUnits(Vector2 vec);

Vector2 getMousePos();

int getMouseX();

int getMouseY();

} // namespace win
