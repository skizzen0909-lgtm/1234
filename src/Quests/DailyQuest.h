// Quests/DailyQuest.h
#ifndef DAILYQUEST_H
#define DAILYQUEST_H

#include "Quest.h"
#include <string>
#include <chrono>

class DailyQuest : public Quest {
private:
    int targetAmount;
    int currentAmount;
    std::string targetEntityId; // ID цели (враг, предмет и т.д.)
    std::time_t creationTime;   // Время создания квеста
    std::time_t expirationTime; // Время истечения (24 часа)
    int rewardGold;
    int rewardExperience;
    
public:
    // Конструктор
    DailyQuest(const std::string& questId,
               const std::string& title,
               const std::string& description,
               QuestType type,
               int targetAmount,
               const std::string& targetEntityId,
               int rewardGold = 100,
               int rewardExperience = 50);
    
    ~DailyQuest() override = default;
    
    // Обновление прогресса
    void updateProgress(int amount);
    
    // Проверка завершения
    bool checkCompletionConditions(Entity* player) override;
    
    // Завершение квеста
    void complete(Entity* player) override;
    
    // Прогресс в строке
    std::string getProgressString() const override;
    
    // Проверка истечения времени
    bool isExpired() const;
    
    // Оставшееся время (в секундах)
    int getRemainingTime() const;
    
    // Геттеры
    int getTargetAmount() const;
    int getCurrentAmount() const;
    const std::string& getTargetEntityId() const;
    int getRewardGold() const;
    int getRewardExperience() const;
    
    // Сброс прогресса (для повторного использования)
    void reset();
    
    // Запрет копирования
    DailyQuest(const DailyQuest&) = delete;
    DailyQuest& operator=(const DailyQuest&) = delete;
};

#endif // DAILYQUEST_H
