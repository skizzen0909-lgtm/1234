// Quests/Quest.cpp

#include "Quest.h"
#include "Entities/Entity.h"
#include <stdexcept>

Quest::Quest(const std::string& questId, const std::string& title,
    const std::string& description, QuestType type)
    : questId(questId), title(title), description(description),
    completed(false), type(type) {
    if (questId.empty()) {
        throw std::invalid_argument("Quest ID cannot be empty");
    }
}

const std::string& Quest::getQuestId() const {
    return questId;
}

const std::string& Quest::getTitle() const {
    return title;
}

const std::string& Quest::getDescription() const {
    return description;
}

bool Quest::isCompleted() const {
    return completed;
}

QuestType Quest::getType() const {
    return type;
}

void Quest::setTitle(const std::string& title) {
    this->title = title;
}

void Quest::setDescription(const std::string& description) {
    this->description = description;
}

void Quest::setCompleted(bool completed) {
    this->completed = completed;
}