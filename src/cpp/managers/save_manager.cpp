#include "managers/save_manager.hpp"

#include "entities/objects/basic_platform.hpp"
#include "entities/objects/object.hpp"
#include "entities/objects/spiky_platform.hpp"
#include "game_state.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace SaveManager {

ObjectP toObject(std::string_view object) {
  if (object == "Platform") {
    return std::make_unique<BasicPlatform>();
  }

  return nullptr;
}

std::string saveObjectsToFile(std::string save_name, GameStateP state) {
  if (!std::filesystem::exists("saves")) {
    std::filesystem::create_directory("saves");
  }
  std::ofstream file("saves/" + save_name + ".block", std::ios::trunc);

  for (const auto &object : state->objects) {
    file << object->getID() << " " << object->save() << "\n";
  }

  file << "END";

  return save_name;
}

std::string loadFromExternalFile(std::string save_name, GameStateP state) {
  if (std::filesystem::path(save_name).extension() != ".block") {
    return "File is not the corrent format";
  }
  std::ifstream file(save_name, std::ios::in);

  if (!file.good()) {
    return "File Not Found";
  }
  state->objects.clear();

  std::string type;
  file >> type;

  while (type != "END") {
    ObjectP object = toObject(type);

    std::string line;
    std::getline(file, line);
    object->load(line);

    state->objects.push_back(std::move(object));

    file >> type;
  }

  std::string stem = std::filesystem::path(save_name).stem().string();
  std::string to = "saves/" + stem + ".block";
  try {
    if (std::filesystem::exists(to)) {
      std::filesystem::remove(to);
    }
    std::filesystem::copy(save_name, to);
  } catch (std::filesystem::filesystem_error e) {
    std::cout << "Could not copy" << "\n";
  }

  return stem;
}

std::string loadObjectsFromFile(std::string save_name, GameStateP state) {
  std::ifstream file("saves/" + save_name + ".block", std::ios::in);

  if (!file.good()) {
    return "File Not Found";
  }
  state->objects.clear();

  std::string type;
  file >> type;

  while (type != "END") {
    ObjectP object = toObject(type);

    std::string line;
    std::getline(file, line);
    object->load(line);

    state->objects.push_back(std::move(object));

    file >> type;
  }

  return save_name;
}

std::string saveScoreToFile(std::string save_name, GameStateP state) {
  if (!std::filesystem::exists("scores")) {
    std::filesystem::create_directory("scores");
  }
  std::ofstream file("scores/" + save_name + ".score", std::ios::trunc);

  file << "MaxHeight " << state->max_height << "\n";

  return save_name;
}

std::string loadScoreFromFile(std::string save_name, GameStateP state) {
  std::ifstream file("scores/" + save_name + ".score", std::ios::in);

  if (!file.good()) {
    return "File Not Found";
  }
  std::string type;
  file >> type;

  float max_height;

  file >> max_height;

  state->max_height = max_height;

  return save_name;
}

std::string saveToFile(std::string save_name, GameStateP state) {
  if (std::string val = saveObjectsToFile(save_name, state); val != save_name) {
    return val;
  }

  return saveScoreToFile(save_name, state);
}

std::string loadFromFile(std::string save_name, GameStateP state) {
  if (std::string val = loadObjectsFromFile(save_name, state);
      val != save_name) {
    return val;
  }

  state->name = save_name;

  return loadObjectsFromFile(save_name, state);
}

void loadDefault(GameStateP state) {
  auto value = loadFromFile(state->name, state);

  if (value == "File Not Found") {
    std::vector<ObjectP> &platforms = state->objects;
    platforms.push_back(std::make_unique<BasicPlatform>(0, 0, 600, 10));
    platforms.push_back(std::make_unique<BasicPlatform>(40, 200, 200, 10));
    platforms.push_back(std::make_unique<SpikyPlatform>(300, 600, 200, 10));

    state->max_height = 0;
  }

  loadScoreFromFile(state->name, state);
}

} // namespace SaveManager