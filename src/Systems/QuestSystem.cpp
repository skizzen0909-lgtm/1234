// Systems/QuestSystem.cpp
#include "QuestSystem.h"
#include "Quests/Quest.h"
#include "Quests/FetchQuest.h"
#include "Quests/QuestChain.h"
#include "Quests/DailyQuest.h"
#include "Entities/Entity.h"
#include "Inventory/Inventory.h"
#include <iostream>
#include <algorithm>
#include <memory>

QuestSystem::QuestSystem() {
}

void QuestSystem::updateQuests(Entity* player) {
    if (!player) return;

    // Очистка истекших ежедневных квестов
    cleanupExpiredDailyQuests();
    
    // Обновление обычных квестов
    for (auto& quest : activeQuests) {
        if (quest && quest->checkCompletionConditions(player)) {
            std::cout << "Квест завершен: " << quest->getTitle() << "\n";
            completeQuest(quest, player);
        }
    }
    
    // Обновление ежедневных квестов
    for (auto& dailyQuest : dailyQuests) {
        if (dailyQuest && !dailyQuest->isExpired() && 
            dailyQuest->checkCompletionConditions(player)) {
            std::cout << "Ежедневный квест завершен: " << dailyQuest->getTitle() << "\n";
            dailyQuest->complete(player);
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
    
    // Удаляем из lookup таблицы
    questLookup.erase(quest->getQuestId());
}

std::shared_ptr<Quest> QuestSystem::findQuestById(const std::string& id) const {
    // Поиск в lookup таблице
    auto it = questLookup.find(id);
    if (it != questLookup.end()) {
        return it->second;
    }
    
    // Поиск в завершенных квестах
    for (const auto& quest : completedQuests) {
        if (quest && quest->getQuestId() == id) {
            return quest;
        }
    }

    return nullptr;
}

void QuestSystem::addQuest(std::shared_ptr<Quest> quest) {
    if (quest && !isQuestActive(quest)) {
        activeQuests.push_back(quest);
        questLookup[quest->getQuestId()] = quest;
    }
}

void QuestSystem::removeQuest(const std::shared_ptr<Quest>& quest) {
    if (!quest) return;

    auto it = std::find(activeQuests.begin(), activeQuests.end(), quest);
    if (it != activeQuests.end()) {
        questLookup.erase(quest->getQuestId());
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

// Методы для цепочек квестов
void QuestSystem::addQuestChain(std::shared_ptr<QuestChain> chain) {
    if (chain) {
        questChains.push_back(chain);
        
        // Добавляем первый квест из цепочки в активные
        auto firstQuest = chain->getCurrentQuest();
        if (firstQuest) {
            addQuest(firstQuest);
        }
    }
}

std::shared_ptr<QuestChain> QuestSystem::getQuestChain(const std::string& chainId) const {
    for (const auto& chain : questChains) {
        if (chain && chain->getChainId() == chainId) {
            return chain;
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<QuestChain>>& QuestSystem::getQuestChains() const {
    return questChains;
}

bool QuestSystem::completeQuestChain(const std::string& chainId, Entity* player) {
    auto chain = getQuestChain(chainId);
    if (!chain || !player) {
        return false;
    }
    
    // Завершаем текущий квест в цепочке
    if (!chain->completeCurrentQuest()) {
        return false;
    }
    
    // Если цепочка завершена
    if (chain->isChainCompleted()) {
        chain->giveChainRewards(player);
        return true;
    }
    
    // Добавляем следующий квест из цепочки
    auto nextQuest = chain->getNextQuest();
    if (nextQuest) {
        addQuest(nextQuest);
    }
    
    return true;
}

// Методы для ежедневных квестов
void QuestSystem::addDailyQuest(std::shared_ptr<DailyQuest> quest) {
    if (quest) {
        dailyQuests.push_back(quest);
    }
}

std::vector<std::shared_ptr<DailyQuest>> QuestSystem::getAvailableDailyQuests() const {
    std::vector<std::shared_ptr<DailyQuest>> available;
    for (const auto& quest : dailyQuests) {
        if (quest && !quest->isExpired() && !quest->isCompleted()) {
            available.push_back(quest);
        }
    }
    return available;
}

std::vector<std::shared_ptr<DailyQuest>> QuestSystem::getCompletedDailyQuests() const {
    std::vector<std::shared_ptr<DailyQuest>> completed;
    for (const auto& quest : dailyQuests) {
        if (quest && quest->isCompleted()) {
            completed.push_back(quest);
        }
    }
    return completed;
}

void QuestSystem::updateDailyQuestProgress(const std::string& targetEntityId, int amount) {
    for (auto& quest : dailyQuests) {
        if (quest && !quest->isExpired() && 
            !quest->isCompleted() && 
            quest->getTargetEntityId() == targetEntityId) {
            quest->updateProgress(amount);
        }
    }
}

void QuestSystem::cleanupExpiredDailyQuests() {
    dailyQuests.erase(
        std::remove_if(dailyQuests.begin(), dailyQuests.end(),
            [](const std::shared_ptr<DailyQuest>& quest) {
                return quest && quest->isExpired();
            }),
        dailyQuests.end()
    );
}

// Статистика
size_t QuestSystem::getTotalActiveQuests() const {
    return activeQuests.size();
}

size_t QuestSystem::getTotalCompletedQuests() const {
    return completedQuests.size();
}

size_t QuestSystem::getTotalQuestChains() const {
    return questChains.size();
}
