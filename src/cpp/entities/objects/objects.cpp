#include "entities/objects/objects.hpp"

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/double_jump_item.hpp"
#include "entities/objects/ghost_platform.hpp"
#include "entities/objects/icy_platform.hpp"
#include "entities/objects/spiky_platform.hpp"
#include "entities/objects/text_object.hpp"

#include <memory>

ObjectP createObject(std::string id) {
  if (id == BasicPlatform::id) {
    return std::make_unique<BasicPlatform>();
  } else if (id == IcyPlatform::id) {
    return std::make_unique<IcyPlatform>();
  } else if (id == SpikyPlatform::id) {
    return std::make_unique<SpikyPlatform>();
  } else if (id == GhostPlatform::id) {
    return std::make_unique<GhostPlatform>();
  } else if (id == TextObject::id) {
    return std::make_unique<TextObject>();
  } else if (id == DoubleJumpItem::id) {
    return std::make_unique<DoubleJumpItem>();
  }

  return nullptr;
}

ObjectP copyObject(Object *object) {
  auto id = object->getID();

  if (id == BasicPlatform::id) {
    return std::make_unique<BasicPlatform>(
        *dynamic_cast<BasicPlatform *>(object));
  } else if (id == IcyPlatform::id) {
    return std::make_unique<IcyPlatform>(*dynamic_cast<IcyPlatform *>(object));
  } else if (id == SpikyPlatform::id) {
    return std::make_unique<SpikyPlatform>(
        *dynamic_cast<SpikyPlatform *>(object));
  } else if (id == GhostPlatform::id) {
    return std::make_unique<GhostPlatform>(
        *dynamic_cast<GhostPlatform *>(object));
  } else if (id == TextObject::id) {
    return std::make_unique<TextObject>(*dynamic_cast<TextObject *>(object));
  } else if (id == DoubleJumpItem::id) {
    return std::make_unique<DoubleJumpItem>(
        *dynamic_cast<DoubleJumpItem *>(object));
  }

  return nullptr;
}
