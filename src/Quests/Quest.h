// Quests/Quest.h
#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <memory>

// Forward declarations
class Entity;

enum class QuestType {
    FETCH,
    KILL,
    DELIVERY,
    EXPLORATION
};

class Quest {
protected:
    std::string questId;
    std::string title;
    std::string description;
    bool completed;
    QuestType type;

public:
    // Конструктор
    Quest(const std::string& questId, const std::string& title,
        const std::string& description, QuestType type);

    // Виртуальный деструктор
    virtual ~Quest() = default;

    // Геттеры
    const std::string& getQuestId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    bool isCompleted() const;
    QuestType getType() const;

    // Сеттеры
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCompleted(bool completed);

    // Виртуальные методы для наследников
    virtual bool checkCompletionConditions(Entity* player) = 0;
    virtual void complete(Entity* player) = 0;
    virtual std::string getProgressString() const = 0;

    // Запрет копирования
    Quest(const Quest&) = delete;
    Quest& operator=(const Quest&) = delete;
};

#endif // QUEST_H