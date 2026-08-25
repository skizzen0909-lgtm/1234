// Events/Event.cpp

#include "Event.h"
#include <stdexcept>

Event::Event(const std::string& eventId)
    : eventId(eventId), handled(false) {
    if (eventId.empty()) {
        throw std::invalid_argument("Event ID cannot be empty");
    }
}

const std::string& Event::getEventId() const {
    return eventId;
}

bool Event::isHandled() const {
    return handled;
}

void Event::setHandled(bool handled) {
    this->handled = handled;
}