#include "entities/background.hpp"

#include "constants.hpp"
#include "managers/resource_manager.hpp"
#include "window.hpp"

#include <raylib.h>

Background::Background() {
  background_texture = ResourceManager::getTexture("background.png");

  SetTextureFilter(*background_texture, TEXTURE_FILTER_TRILINEAR);
}

void Background::update(float height) {
  int pos =
      float((height + constants::window_height) / background_texture->height);

  pos1 = (pos - 1) * background_texture->height;
  pos2 = pos * background_texture->height;
}

void Background::draw() {
  win::drawTexture(background_texture, {0, pos1});
  win::drawTexture(background_texture, {0, pos2});
}
