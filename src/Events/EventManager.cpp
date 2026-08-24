// Events/EventsManager.cpp
#include "EventManager.h"
#include <algorithm>

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(const std::string& eventType, const Listener& listener) {
    listeners[eventType].push_back(listener);
}

void EventManager::unsubscribe(const std::string& eventType, const Listener& listener) {
    auto& list = listeners[eventType];
    list.erase(std::remove_if(list.begin(), list.end(),
        [&listener](const Listener& l) {
            return l.target_type() == listener.target_type();
        }), list.end());
}

void EventManager::emit(const std::shared_ptr<Event>& event) {
    if (!event || event->isHandled()) return;

    const auto& type = event->getEventId();
    if (auto it = listeners.find(type); it != listeners.end()) {
        for (const auto& listener : it->second) {
            listener(event);
        }
    }

    // Также рассылаем по базовому типу (например, "combat")
    size_t pos = type.find('_');
    if (pos != std::string::npos) {
        std::string baseType = type.substr(0, pos);
        if (auto it = listeners.find(baseType); it != listeners.end()) {
            for (const auto& listener : it->second) {
                listener(event);
            }
        }
    }
}