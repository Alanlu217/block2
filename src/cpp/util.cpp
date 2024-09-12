#include "util.hpp"
#include "entities/objects/basic_platform.hpp"
#include "entities/objects/icy_platform.hpp"
#include "entities/objects/spiky_platform.hpp"
#include <memory>

ObjectP createObject(std::string id) {
  if (id == "Platform") {
    return std::make_unique<BasicPlatform>();
  } else if (id == "IcyPlatform") {
    return std::make_unique<IcyPlatform>();
  } else if (id == "SpikyPlatform") {
    return std::make_unique<SpikyPlatform>();
  }

  return nullptr;
}

ObjectP copyObject(Object *object) {
  auto id = object->getID();

  if (id == "Platform") {
    return std::make_unique<BasicPlatform>(
        *dynamic_cast<BasicPlatform *>(object));
  } else if (id == "IcyPlatform") {
    return std::make_unique<IcyPlatform>(*dynamic_cast<IcyPlatform *>(object));
  } else if (id == "SpikyPlatform") {
    return std::make_unique<SpikyPlatform>(
        *dynamic_cast<SpikyPlatform *>(object));
  }

  return nullptr;
}
