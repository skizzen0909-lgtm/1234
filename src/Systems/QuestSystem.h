// Systems/QuestSystem.h
#ifndef QUESTSYSTEM_H
#define QUESTSYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include "Quests/Quest.h"

class Entity;

class QuestSystem {
private:
    std::vector<std::shared_ptr<Quest>> activeQuests;
    std::vector<std::shared_ptr<Quest>> completedQuests;

public:
    QuestSystem();
    ~QuestSystem() = default;

    void addQuest(std::shared_ptr<Quest> quest);
    void removeQuest(const std::shared_ptr<Quest>& quest);
    bool isQuestActive(const std::shared_ptr<Quest>& quest) const;
    const std::vector<std::shared_ptr<Quest>>& getActiveQuests() const;

    void completeQuest(std::shared_ptr<Quest> quest, Entity* player);
    const std::vector<std::shared_ptr<Quest>>& getCompletedQuests() const;

    std::shared_ptr<Quest> findQuestById(const std::string& id) const;
    std::vector<std::shared_ptr<Quest>> findQuestsByType(QuestType type) const;

    void updateQuests(Entity* player);

    QuestSystem(const QuestSystem&) = delete;
    QuestSystem& operator=(const QuestSystem&) = delete;
};

#endif // QUESTSYSTEM_H