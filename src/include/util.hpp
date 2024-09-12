#pragma once

#include "entities/objects/object.hpp"

#include <string>

ObjectP createObject(std::string id);

ObjectP copyObject(Object *object);
