#include "managers/save_manager.hpp"
#include "entities/basic_platform.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace SaveManager {

std::string toString(const BasicPlatform &platform) {
  std::stringstream s;
  s << platform.rect.x << " " << platform.rect.y << " " << platform.rect.width
    << " " << platform.rect.height;

  return s.str();
}

std::string saveToFile(std::string save_name, GameStateP state) {
  std::filesystem::create_directory("saves");
  std::ofstream file("saves/" + save_name + ".block", std::ios::trunc);

  for (const auto &platform : state->entities.platforms) {
    file << "Platform " << toString(platform) << "\n";
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
  state->entities.platforms.clear();

  std::string type;

  file >> type;

  while (type != "END") {
    if (type == "Platform") {
      float x, y, width, height;

      file >> x >> y >> width >> height;

      state->entities.platforms.push_back({x, y, width, height});
    }

    file >> type;
  }

  return save_name;
}

void loadDefault(GameStateP state) {
  auto value = loadFromFile("default", state);

  if (value == "File Not Found") {
    std::vector<BasicPlatform> &platforms = state->entities.platforms;
    platforms.push_back({0, 0, 600, 10});
    platforms.push_back({40, 200, 200, 10});
    platforms.push_back({300, 600, 200, 10});
  }
}

} // namespace SaveManager