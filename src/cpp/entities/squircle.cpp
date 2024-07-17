#include "entities/squircle.hpp"
#include "constants.hpp"
#include "managers/resource_manager.hpp"
#include "raylib.h"
#include "window.hpp"

#include <iostream>

Squircle::Squircle() {
  squircle_texture = ResourceManager::getTexture("squircle.png");
  GenTextureMipmaps(squircle_texture.get());
  SetTextureFilter(*squircle_texture, TEXTURE_FILTER_TRILINEAR);

  pos = {0, 0};
}

Squircle::~Squircle() {}

void Squircle::draw() { win::drawTexture(squircle_texture, pos); }

Rectangle Squircle::getBounds() { return {pos.x, pos.y, width, width}; }
