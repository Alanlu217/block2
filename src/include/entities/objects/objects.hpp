#pragma once

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/double_jump_item.hpp"
#include "entities/objects/ghost_platform.hpp"
#include "entities/objects/icy_platform.hpp"
#include "entities/objects/object.hpp"
#include "entities/objects/spiky_platform.hpp"
#include "entities/objects/text_object.hpp"

#include <string>

const static char *object_options[] = {
    BasicPlatform::id.c_str(), SpikyPlatform::id.c_str(),
    IcyPlatform::id.c_str(),   GhostPlatform::id.c_str(),
    TextObject::id.c_str(),    DoubleJumpItem::id.c_str()};

ObjectP createObject(std::string id);

ObjectP copyObject(Object *object);
