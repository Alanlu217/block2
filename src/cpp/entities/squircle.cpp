#include "entities/squircle.hpp"
#include "constants.hpp"
#include "managers/resource_manager.hpp"
#include "raylib.h"
#include "window.hpp"

#include <iostream>

Squircle::Squircle() {
  squircle_texture = ResourceManager::getTexture("squircle.png");
  squircle_texture->GenMipmaps();
  squircle_texture->SetFilter(TextureFilter::TEXTURE_FILTER_TRILINEAR);

  pos = {0, 0};
}

Squircle::~Squircle() {}

void Squircle::draw(raylib::Window &window) {
  win::drawTexture(squircle_texture, pos);
}

Vector2 Squircle::getPosition() { return pos; }

int Squircle::getWidth() { return width; }

void Squircle::setPosition(Vector2 pos) { this->pos = pos; }

Rectangle Squircle::getBounds() {
  return {pos.x, pos.y, pos.x + width, pos.y + width};
}