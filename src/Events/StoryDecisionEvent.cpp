// Events/StoryDecisionEvent.cpp

#include "StoryDecisionEvent.h"
#include "Decisions/Decision.h"
#include "Entities/Entity.h"
#include <iostream>
#include <algorithm>
#include <memory>

StoryDecisionEvent::StoryDecisionEvent(const std::string& storyId, const std::string& storyText,
    Entity* triggeringEntity)
    : Event("story_decision_" + storyId), storyId(storyId), storyText(storyText),
    triggeringEntity(triggeringEntity) {
    if (storyId.empty()) {
        throw std::invalid_argument("Story ID cannot be empty");
    }
}

void StoryDecisionEvent::execute() {
    if (isHandled()) {
        return;
    }

    std::cout << "\n=== Историческое событие ===" << std::endl;
    std::cout << storyText << std::endl;

    if (!availableDecisions.empty()) {
        std::cout << "\nДоступные варианты:" << std::endl;
        for (size_t i = 0; i < availableDecisions.size(); ++i) {
            const auto& decision = availableDecisions[i];
            if (decision) {
                std::cout << (i + 1) << ". " << decision->getDescription() << std::endl;
            }
        }
    }

    setHandled(true);
}

std::string StoryDecisionEvent::getDescription() const {
    return "Историческое событие: " + storyText;
}

void StoryDecisionEvent::addDecision(std::shared_ptr<Decision> decision) {
    if (decision) {
        availableDecisions.push_back(decision);
    }
}

void StoryDecisionEvent::removeDecision(const std::shared_ptr<Decision>& decision) {
    if (!decision) return;

    auto it = std::find(availableDecisions.begin(), availableDecisions.end(), decision);
    if (it != availableDecisions.end()) {
        availableDecisions.erase(it);
    }
}

const std::vector<std::shared_ptr<Decision>>& StoryDecisionEvent::getAvailableDecisions() const {
    return availableDecisions;
}

const std::string& StoryDecisionEvent::getStoryId() const {
    return storyId;
}

const std::string& StoryDecisionEvent::getStoryText() const {
    return storyText;
}

Entity* StoryDecisionEvent::getTriggeringEntity() const {
    return triggeringEntity;
}

void StoryDecisionEvent::setStoryText(const std::string& text) {
    storyText = text;
}

void StoryDecisionEvent::setTriggeringEntity(Entity* entity) {
    triggeringEntity = entity;
}