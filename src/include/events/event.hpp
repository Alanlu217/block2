#pragma once

#include "./close_window_event.hpp"
#include "events/change_view_event.hpp"
#include "events/toggle_debug_event.hpp"

#include <variant>

typedef std::variant<CloseWindowEvent, ChangeViewEvent, ToggleDebugEvent> Event;

enum EventTypes { CloseWindowEvent, ChangeViewEvent, ToggleDebugEvent };