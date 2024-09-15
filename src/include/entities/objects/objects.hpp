#pragma once

#include "entities/objects/object.hpp"

#include <string>

const static char *object_options[4] = {"Platform", "SpikyPlatform",
                                        "IcyPlatform", "Text"};

ObjectP createObject(std::string id);

ObjectP copyObject(Object *object);
