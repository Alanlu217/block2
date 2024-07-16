#pragma once

#include "events/event.hpp"

#include <functional>
#include <map>
#include <vector>

typedef std::function<void(Event)> EventListener;
typedef unsigned int EventListenerID;

class EventManager {
private:
  static std::vector<Event> active_events;
  static std::map<EventTypes,
                  std::vector<std::pair<EventListenerID, EventListener>>>
      event_listeners;

  static EventListenerID current_id;

public:
  EventManager() = delete;

  static EventListenerID addListener(EventTypes event_type,
                                     EventListener event_listener);

  static void removeListener(EventTypes event_type,
                             EventListenerID listener_id);

  static void triggerEvent(Event event);

  static void update();
};