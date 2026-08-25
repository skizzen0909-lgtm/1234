// Decisions/Decision.cpp

#include "Decision.h"
#include "Entity.h"
#include "Events/Event.h"
#include <iostream>
#include <algorithm>
#include <memory>
Decision::Decision(const std::string& decisionId, const std::string& description)
    : decisionId(decisionId), description(description) {
    if (decisionId.empty()) {
        throw std::invalid_argument("Decision ID cannot be empty");
    }
}

const std::string& Decision::getDecisionId() const {
    return decisionId;
}

const std::string& Decision::getDescription() const {
    return description;
}

const std::vector<std::shared_ptr<Event>>& Decision::getConsequences() const {
    return consequences;
}

void Decision::setDescription(const std::string& description) {
    this->description = description;
}

void Decision::addConsequence(std::shared_ptr<Event> event) {
    if (event) {
        consequences.push_back(event);
    }
}

void Decision::removeConsequence(const std::shared_ptr<Event>& event) {
    if (!event) return;

    auto it = std::find(consequences.begin(), consequences.end(), event);
    if (it != consequences.end()) {
        consequences.erase(it);
    }
}
