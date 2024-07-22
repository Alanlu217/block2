#include "managers/save_manager.hpp"

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/object.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

namespace SaveManager {

ObjectP toObject(std::string_view object) {
  if (object == "Platform") {
    return std::make_unique<BasicPlatform>();
  }

  return nullptr;
}

std::string toString(const BasicPlatform &platform) {
  std::stringstream s;
  s << platform.rect.x << " " << platform.rect.y << " " << platform.rect.width
    << " " << platform.rect.height;

  return s.str();
}

std::string saveToFile(std::string save_name, GameStateP state) {
  std::filesystem::create_directory("saves");
  std::ofstream file("saves/" + save_name + ".block", std::ios::trunc);

  for (const auto &object : state->objects) {
    file << object->getID() << " " << object->save() << "\n";
  }

  file << "END";

  return save_name;
}

std::string loadFromFile(std::string save_name, GameStateP state) {
  std::filesystem::create_directory("saves");
  std::ifstream file("saves/" + save_name + ".block", std::ios::in);

  if (!file.good()) {
    return "File Not Found";
  }
  state->objects.clear();

  std::string type;

  for (; type != "END"; file >> type) {
    ObjectP object = toObject(type);

    std::string line;
    std::getline(file, line);
    object->load(line);

    state->objects.push_back(std::move(object));
  }

  return save_name;
}

void loadDefault(GameStateP state) {
  auto value = loadFromFile("default", state);

  if (value == "File Not Found") {
    std::vector<ObjectP> &platforms = state->objects;
    platforms.push_back(std::make_unique<BasicPlatform>(0, 0, 600, 10));
    platforms.push_back(std::make_unique<BasicPlatform>(40, 200, 200, 10));
    platforms.push_back(std::make_unique<BasicPlatform>(300, 600, 200, 10));
  }
}

} // namespace SaveManager