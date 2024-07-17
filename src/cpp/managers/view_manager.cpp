#include "managers/view_manager.hpp"
#include "views/view.hpp"
#include <algorithm>

std::vector<ViewP> ViewManager::active_views;
ViewMap ViewManager::views;

void ViewManager::init(const std::string &start_view, ViewMap views) {
  ViewManager::views = views;

  views[start_view]->init();
  active_views.push_back(views[start_view]);
}

void ViewManager::update(const double delta_time) {
  for (auto &view : active_views) {
    view->update(delta_time);
  }
}

void ViewManager::render(const double delta_time) {
  for (auto &view : active_views) {
    view->render(delta_time);
  }
}

void ViewManager::setView(const std::string &view) {
  std::ranges::for_each(active_views, [](auto &item) { item->close(); });
  active_views.clear();

  views[view]->init();
  active_views.push_back(views[view]);
}

void ViewManager::addView(const std::string &view) {
  views[view]->init();
  active_views.push_back(views[view]);
}

void ViewManager::closeView(const std::string &view) {
  views[view]->close();

  active_views.erase(std::ranges::remove(active_views, views[view]).begin(),
                     active_views.end());
}

ViewMap &ViewManager::getView() { return views; }

void ViewManager::shutdown() {
  for (auto &view : active_views) {
    view->close();
  }
}