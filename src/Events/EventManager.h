// Events/EventsManager.h

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Event.h"

class EventManager {
public:
    using Listener = std::function<void(const std::shared_ptr<Event>&)>;

    static EventManager& getInstance();

    void subscribe(const std::string& eventType, const Listener& listener);
    void unsubscribe(const std::string& eventType, const Listener& listener);
    void emit(const std::shared_ptr<Event>& event);

    // Удаляем копирование
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

private:
    EventManager() = default;
    ~EventManager() = default;

    std::unordered_map<std::string, std::vector<Listener>> listeners;
};

#endif // EVENTMANAGER_H