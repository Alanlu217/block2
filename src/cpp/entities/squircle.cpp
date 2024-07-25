#include "entities/squircle.hpp"

#include "constants.hpp"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <raylib.h>

Squircle::Squircle() {
  squircle_texture = ResourceManager::getTexture("squircle.png");
  GenTextureMipmaps(squircle_texture.get());
  SetTextureFilter(*squircle_texture, TEXTURE_FILTER_TRILINEAR);

  pos = {0, 0};
}

void Squircle::draw() {
  // win::drawTexturePro(
  //     squircle_texture,
  //     {0, 0, float(squircle_texture->width),
  //     float(squircle_texture->height)}, {pos.x, pos.y, width, width}, {0,
  //     width / 2});

  DrawTexturePro(
      *squircle_texture,
      {0, 0, float(squircle_texture->width), float(squircle_texture->height)},
      {pos.x, static_cast<float>(win::toWindowUnits(pos.y, width)), width,
       width},
      {0, 0}, 0.0f, WHITE);
}

Rectangle Squircle::getBounds() { return {pos.x, pos.y, width, width}; }
