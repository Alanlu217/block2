#include "managers/resource_manager.hpp"

#include <raylib.h>
#include <sstream>

std::map<std::string, TextureP> ResourceManager::textures;
std::map<std::string, FontP> ResourceManager::fonts;

TextureP ResourceManager::getTexture(const std::string &filepath) {
  const auto texture = textures.find(filepath);

  if (texture == textures.end()) {
    auto temp = std::make_shared<Texture>();
    *temp = LoadTexture((ASSETS_DIR + filepath).c_str());
    textures[filepath] = temp;

    return temp;
  }

  return texture->second;
}

FontP ResourceManager::getFont(const std::string &filepath) {
  const auto font = fonts.find(filepath);

  if (font == fonts.end()) {
    auto temp = std::make_shared<Font>();
    *temp = LoadFontEx((ASSETS_DIR + filepath).c_str(), 200, 0, 250);
    fonts[filepath] = temp;

    return temp;
  }

  return font->second;
}

std::string ResourceManager::listLoadedResources() {
  std::stringstream ss;
  ss << "Fonts: { ";

  for (const auto &font : fonts) {
    ss << font.first << " ";
  }

  ss << " }\nTextures: { ";

  for (const auto &texture : textures) {
    ss << texture.first << " ";
  }
  ss << " }";

  return ss.str();
}

void ResourceManager::clearUnused() {
  std::erase_if(fonts,
                [](const auto &item) { return item.second.use_count() == 1; });
  std::erase_if(textures,
                [](const auto &item) { return item.second.use_count() == 1; });
}

void ResourceManager::shutdown() {
  fonts.clear();
  textures.clear();
}
