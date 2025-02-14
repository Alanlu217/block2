#pragma once

#include <map>
#include <memory>
#include <raylib.h>

typedef std::shared_ptr<Font> FontP;
typedef std::shared_ptr<Texture> TextureP;

class ResourceManager {
private:
  static std::map<std::string, FontP> fonts;
  static std::map<std::string, TextureP> textures;

public:
  ResourceManager() = delete;

  static TextureP getTexture(const std::string &filepath);
  static FontP getFont(const std::string &filepath);

  static std::string listLoadedResources();

  static void clearUnused();

  static void shutdown();
};