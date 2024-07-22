#include "entities/objects/basic_platform.hpp"

#include "constants.hpp"
#include "entities/objects/object.hpp"
#include "window.hpp"

#include <raylib.h>
#include <sstream>
#include <string_view>

BasicPlatform::BasicPlatform() { rect = {0, 0, 0, 0}; }
BasicPlatform::BasicPlatform(const BasicPlatform &platform)
    : rect(platform.rect) {}
BasicPlatform::BasicPlatform(float x, float y, float width, float height) {
  rect = {x, y, width, height};
}

void BasicPlatform::draw() { win::drawRectangle(rect, WHITE); }

BoundsP BasicPlatform::getBounds() { return &rect; }

void BasicPlatform::setPosition(float x, float y) {
  rect.x = x;
  rect.y = y;
}

void BasicPlatform::update() {}

void BasicPlatform::load(std::string_view object) {
  std::stringstream s{std::string{object}};

  float x, y, width, height;

  s >> x >> y >> width >> height;

  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
}

std::string_view BasicPlatform::save() {
  std::stringstream s;
  s << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;

  return s.view();
}