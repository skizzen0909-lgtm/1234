// Systems/QuestSystem.cpp
#include "QuestSystem.h"
#include "Quests/Quest.h"
#include "Quests/FetchQuest.h"
#include "Entities/Entity.h"
#include "Inventory/Inventory.h"
#include <iostream>
#include <algorithm>
#include <memory>
void QuestSystem::updateQuests(Entity* player) {
    if (!player) return;

    for (auto& quest : activeQuests) {
        if (quest && quest->checkCompletionConditions(player)) {
            std::cout << "Квест завершен: " << quest->getTitle() << "\n";
            completeQuest(quest, player);
        }
    }
}

void QuestSystem::completeQuest(std::shared_ptr<Quest> quest, Entity* player) {
    if (!quest || !player) return;

    quest->complete(player);
    quest->setCompleted(true);

    // Перемещаем из активных в завершенные
    auto it = std::find(activeQuests.begin(), activeQuests.end(), quest);
    if (it != activeQuests.end()) {
        activeQuests.erase(it);
        completedQuests.push_back(quest);
    }
}

std::shared_ptr<Quest> QuestSystem::findQuestById(const std::string& id) const {
    for (const auto& quest : activeQuests) {
        if (quest && quest->getQuestId() == id) {
            return quest;
        }
    }

    for (const auto& quest : completedQuests) {
        if (quest && quest->getQuestId() == id) {
            return quest;
        }
    }

    return nullptr;
}

QuestSystem::QuestSystem() {
}

void QuestSystem::addQuest(std::shared_ptr<Quest> quest) {
    if (quest && !isQuestActive(quest)) {
        activeQuests.push_back(quest);
    }
}

void QuestSystem::removeQuest(const std::shared_ptr<Quest>& quest) {
    if (!quest) return;

    auto it = std::find(activeQuests.begin(), activeQuests.end(), quest);
    if (it != activeQuests.end()) {
        activeQuests.erase(it);
    }
}

bool QuestSystem::isQuestActive(const std::shared_ptr<Quest>& quest) const {
    if (!quest) return false;
    return std::find(activeQuests.begin(), activeQuests.end(), quest) != activeQuests.end();
}

const std::vector<std::shared_ptr<Quest>>& QuestSystem::getActiveQuests() const {
    return activeQuests;
}


const std::vector<std::shared_ptr<Quest>>& QuestSystem::getCompletedQuests() const {
    return completedQuests;
}


std::vector<std::shared_ptr<Quest>> QuestSystem::findQuestsByType(QuestType type) const {
    std::vector<std::shared_ptr<Quest>> result;

    for (const auto& quest : activeQuests) {
        if (quest && quest->getType() == type) {
            result.push_back(quest);
        }
    }

    for (const auto& quest : completedQuests) {
        if (quest && quest->getType() == type) {
            result.push_back(quest);
        }
    }

    return result;
}
