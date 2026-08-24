// World/WorldEvent.cpp
#include "WorldEvent.h"
#include <unordered_map>

WorldEvent::WorldEvent(const std::string& id,
                       const std::string& name,
                       const std::string& desc,
                       WorldEventType type,
                       float duration,
                       int priority)
    : eventId(id),
      eventName(name),
      description(desc),
      type(type),
      active(false),
      duration(duration),
      priority(priority) {
}

void WorldEvent::activate() {
    active = true;
    startTime = std::chrono::steady_clock::now();
}

void WorldEvent::deactivate() {
    active = false;
}

void WorldEvent::update(float deltaTime) {
    if (!active) {
        return;
    }
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
    
    if (elapsedTime >= duration) {
        deactivate();
    }
}

bool WorldEvent::isActive() const {
    return active && !isExpired();
}

bool WorldEvent::isExpired() const {
    if (!active) {
        return true;
    }
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
    
    return elapsedTime >= duration;
}

std::string WorldEvent::getEventDescription() const {
    return description;
}

const std::string& WorldEvent::getEventId() const {
    return eventId;
}

const std::string& WorldEvent::getEventName() const {
    return eventName;
}

WorldEventType WorldEvent::getEventType() const {
    return type;
}

float WorldEvent::getDuration() const {
    return duration;
}

float WorldEvent::getRemainingTime() const {
    if (!active) {
        return 0.0f;
    }
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
    
    return std::max(0.0f, duration - elapsedTime);
}

int WorldEvent::getPriority() const {
    return priority;
}

// Реализация фабрики
WorldEventFactory& WorldEventFactory::getInstance() {
    static WorldEventFactory instance;
    return instance;
}

void WorldEventFactory::registerEvent(WorldEventType type, EventCreator creator) {
    creators[type] = creator;
}

std::shared_ptr<WorldEvent> WorldEventFactory::createEvent(WorldEventType type) {
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    }
    return nullptr;
}
