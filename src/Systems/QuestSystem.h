// Systems/QuestSystem.h
#ifndef QUESTSYSTEM_H
#define QUESTSYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "Quests/Quest.h"
#include "Quests/QuestChain.h"
#include "Quests/DailyQuest.h"

class Entity;

class QuestSystem {
private:
    std::vector<std::shared_ptr<Quest>> activeQuests;
    std::vector<std::shared_ptr<Quest>> completedQuests;
    std::vector<std::shared_ptr<QuestChain>> questChains;
    std::vector<std::shared_ptr<DailyQuest>> dailyQuests;
    std::unordered_map<std::string, std::shared_ptr<Quest>> questLookup;
    
    // Очистка истекших ежедневных квестов
    void cleanupExpiredDailyQuests();

public:
    QuestSystem();
    ~QuestSystem() = default;

    // Управление обычными квестами
    void addQuest(std::shared_ptr<Quest> quest);
    void removeQuest(const std::shared_ptr<Quest>& quest);
    bool isQuestActive(const std::shared_ptr<Quest>& quest) const;
    const std::vector<std::shared_ptr<Quest>>& getActiveQuests() const;

    void completeQuest(std::shared_ptr<Quest> quest, Entity* player);
    const std::vector<std::shared_ptr<Quest>>& getCompletedQuests() const;

    std::shared_ptr<Quest> findQuestById(const std::string& id) const;
    std::vector<std::shared_ptr<Quest>> findQuestsByType(QuestType type) const;

    // Управление цепочками квестов
    void addQuestChain(std::shared_ptr<QuestChain> chain);
    std::shared_ptr<QuestChain> getQuestChain(const std::string& chainId) const;
    const std::vector<std::shared_ptr<QuestChain>>& getQuestChains() const;
    bool completeQuestChain(const std::string& chainId, Entity* player);
    
    // Управление ежедневными квестами
    void addDailyQuest(std::shared_ptr<DailyQuest> quest);
    std::vector<std::shared_ptr<DailyQuest>> getAvailableDailyQuests() const;
    std::vector<std::shared_ptr<DailyQuest>> getCompletedDailyQuests() const;
    void updateDailyQuestProgress(const std::string& targetEntityId, int amount);
    
    // Обновление всех квестов
    void updateQuests(Entity* player);
    
    // Статистика
    size_t getTotalActiveQuests() const;
    size_t getTotalCompletedQuests() const;
    size_t getTotalQuestChains() const;

    QuestSystem(const QuestSystem&) = delete;
    QuestSystem& operator=(const QuestSystem&) = delete;
};

#endif // QUESTSYSTEM_H