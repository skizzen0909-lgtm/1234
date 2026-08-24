// Quests/FetchQuest.h
#ifndef FETCHQUEST_H
#define FETCHQUEST_H

#include "Quest.h"
#include <string>
#include <memory>

// Forward declarations
class Item;
class Entity;

class FetchQuest : public Quest {
private:
    std::shared_ptr<Item> requiredItem;
    int requiredQuantity;
    int currentQuantity;

public:
    // Конструктор
    FetchQuest(const std::string& questId, const std::string& title,
        const std::string& description, std::shared_ptr<Item> requiredItem,
        int requiredQuantity);

    // Реализация виртуальных методов
    bool checkCompletionConditions(Entity* player) override;
    void complete(Entity* player) override;
    std::string getProgressString() const override;

    // Геттеры
    std::shared_ptr<Item> getRequiredItem() const;
    int getRequiredQuantity() const;
    int getCurrentQuantity() const;

    // Сеттеры
    void setCurrentQuantity(int quantity);
};

#endif // FETCHQUEST_H