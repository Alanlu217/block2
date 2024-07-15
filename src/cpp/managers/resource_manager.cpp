#include "managers/resource_manager.hpp"
#include "RenderTexture.hpp"

#include <sstream>

std::map<std::string, TextureP> ResourceManager::textures;
std::map<std::string, FontP> ResourceManager::fonts;

TextureP ResourceManager::getTexture(const std::string &filepath) {
  const auto texture = textures.find(filepath);

  if (texture == textures.end()) {
    auto temp = std::make_shared<raylib::Texture>();
    temp->Load(ASSETS_DIR + filepath);
    textures[filepath] = temp;

    return temp;
  }

  return texture->second;
}

FontP ResourceManager::getFont(const std::string &filepath) {
  const auto font = fonts.find(filepath);

  if (font == fonts.end()) {
    auto temp = std::shared_ptr<raylib::Font>();
    temp->Load(ASSETS_DIR + filepath);
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

  ss << " }\n Textures: { ";

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