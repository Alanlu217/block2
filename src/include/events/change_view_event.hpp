#pragma once

#include <string>

struct ChangeViewEvent {
  std::string old_view, new_view;
};