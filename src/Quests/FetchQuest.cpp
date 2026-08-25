// Quests/FetchQuest.cpp
#include "FetchQuest.h"
#include "Items/Item.h"
#include "Entities/Entity.h"
#include "Inventory/Inventory.h"
#include <sstream>
#include <memory>
FetchQuest::FetchQuest(const std::string& questId, const std::string& title,
    const std::string& description, std::shared_ptr<Item> requiredItem,
    int requiredQuantity)
    : Quest(questId, title, description, QuestType::FETCH),
    requiredItem(requiredItem), requiredQuantity(requiredQuantity), currentQuantity(0) {
    if (!requiredItem) {
        throw std::invalid_argument("Required item cannot be null");
    }
    if (requiredQuantity <= 0) {
        throw std::invalid_argument("Required quantity must be positive");
    }
}

bool FetchQuest::checkCompletionConditions(Entity* player) {
    if (!player) {
        return false;
    }
    // Получаем инвентарь игрока
    const std::vector<std::shared_ptr<Item>>& inventory = player->getInventory();
    // Здесь должна быть логика подсчета количества предметов в инвентаре
    currentQuantity = 0;
    for (const auto& item : inventory) {
        if (item && item->getId() == requiredItem->getId()) {
            currentQuantity++;
        }
    }
    return currentQuantity >= requiredQuantity;
}

void FetchQuest::complete(Entity* player) {
    if (!player || !checkCompletionConditions(player)) {
        return;
    }
    // Здесь должна быть логика удаления предметов из инвентаря
    // и выдачи наград
    setCompleted(true);
}

std::string FetchQuest::getProgressString() const {
    std::ostringstream oss;
    oss << "Собрано: " << currentQuantity << "/" << requiredQuantity;
    return oss.str();
}

std::shared_ptr<Item> FetchQuest::getRequiredItem() const {
    return requiredItem;
}

int FetchQuest::getRequiredQuantity() const {
    return requiredQuantity;
}

int FetchQuest::getCurrentQuantity() const {
    return currentQuantity;
}

void FetchQuest::setCurrentQuantity(int quantity) {
    if (quantity >= 0) {
        currentQuantity = quantity;
    }
}