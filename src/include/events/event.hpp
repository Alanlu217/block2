#pragma once

#include "events/change_view_event.hpp"
#include "events/close_window_event.hpp"
#include "events/start_test_event.hpp"
#include "events/toggle_debug_event.hpp"

#include <variant>

typedef std::variant<CloseWindowEvent, ChangeViewEvent, ToggleDebugEvent,
                     StartTestEvent>
    Event;

enum EventTypes {
  CloseWindowEvent,
  ChangeViewEvent,
  ToggleDebugEvent,
  StartTestEvent
};