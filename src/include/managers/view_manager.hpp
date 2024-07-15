#pragma once

#include "views/view.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::map<std::string, ViewP> ViewMap;

class ViewManager {
private:
  static std::vector<ViewP> active_views;

  static ViewMap views;

public:
  ViewManager() = delete;

  static void init(const std::string &start_view, ViewMap views);

  static void update(raylib::Window &window, const double delta_time);

  static void render(raylib::Window &window, const double delta_time);

  static void setView(const std::string &view);

  static void addView(const std::string &view);

  static void closeView(const std::string &view);

  static ViewMap &getView();

  static void shutdown();
};