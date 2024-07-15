#include "managers/event_manager.hpp"

#include "events/event.hpp"

#include <algorithm>
#include <utility>

std::vector<Event> EventManager::active_events;
std::map<EventTypes, std::vector<std::pair<EventListenerID, EventListener>>>
    EventManager::event_listeners;

EventListenerID EventManager::currentID = 0;

EventListenerID EventManager::addListener(EventTypes event_type,
                                          EventListener event_listener) {
  if (event_listeners.count(event_type) != 0) {
    event_listeners[event_type].push_back(
        std::make_pair(currentID++, event_listener));
  } else {
    event_listeners[event_type] = {std::make_pair(currentID++, event_listener)};
  }

  return currentID++;
}

void EventManager::removeListener(EventTypes event_type,
                                  EventListenerID listener_id) {
  if (event_listeners.count(event_type) != 0) {
    std::vector<std::pair<EventListenerID, EventListener>> &vec =
        event_listeners[event_type];

    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [listener_id](const auto &listener) {
                               return listener.first == listener_id;
                             }),
              vec.end());
  }
}

void EventManager::triggerEvent(Event event) { active_events.push_back(event); }

void EventManager::update() {
  for (const auto &event : active_events) {
    EventTypes type = (EventTypes)event.index();
    for (const auto &func : event_listeners[type]) {
      func.second(event);
    }
  }
}