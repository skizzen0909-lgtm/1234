// Quests/DailyQuest.cpp
#include "DailyQuest.h"
#include "Entities/Entity.h"

DailyQuest::DailyQuest(const std::string& questId,
                       const std::string& title,
                       const std::string& description,
                       QuestType type,
                       int targetAmount,
                       const std::string& targetEntityId,
                       int rewardGold,
                       int rewardExperience)
    : Quest(questId, title, description, type),
      targetAmount(targetAmount),
      currentAmount(0),
      targetEntityId(targetEntityId),
      rewardGold(rewardGold),
      rewardExperience(rewardExperience) {
    
    // Установка времени создания и истечения (24 часа)
    creationTime = std::time(nullptr);
    expirationTime = creationTime + (24 * 60 * 60); // 24 часа в секундах
    
    // Помечаем квест как невыполненный
    setCompleted(false);
}

void DailyQuest::updateProgress(int amount) {
    currentAmount += amount;
    if (currentAmount >= targetAmount) {
        currentAmount = targetAmount;
        setCompleted(true);
    }
}

bool DailyQuest::checkCompletionConditions(Entity* player) {
    if (!player) {
        return false;
    }
    
    // Проверка истечения времени
    if (isExpired()) {
        return false;
    }
    
    // Проверка выполнения условия по количеству
    return currentAmount >= targetAmount;
}

void DailyQuest::complete(Entity* player) {
    if (!player || !checkCompletionConditions(player)) {
        return;
    }
    
    // Выдача наград
    player->gainExperience(rewardExperience);
    
    // Здесь можно добавить выдачу золота через систему инвентаря
    // if (auto inventory = player->getInventory()) {
    //     inventory->addGold(rewardGold);
    // }
    
    setCompleted(true);
}

std::string DailyQuest::getProgressString() const {
    return std::to_string(currentAmount) + "/" + std::to_string(targetAmount);
}

bool DailyQuest::isExpired() const {
    return std::time(nullptr) > expirationTime;
}

int DailyQuest::getRemainingTime() const {
    auto now = std::time(nullptr);
    if (now >= expirationTime) {
        return 0;
    }
    return static_cast<int>(expirationTime - now);
}

int DailyQuest::getTargetAmount() const {
    return targetAmount;
}

int DailyQuest::getCurrentAmount() const {
    return currentAmount;
}

const std::string& DailyQuest::getTargetEntityId() const {
    return targetEntityId;
}

int DailyQuest::getRewardGold() const {
    return rewardGold;
}

int DailyQuest::getRewardExperience() const {
    return rewardExperience;
}

void DailyQuest::reset() {
    currentAmount = 0;
    setCompleted(false);
    creationTime = std::time(nullptr);
    expirationTime = creationTime + (24 * 60 * 60);
}
