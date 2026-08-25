// Events/StoryDecisionEvent.h
#ifndef STORYDECISIONEVENT_H
#define STORYDECISIONEVENT_H

#include "Event.h"
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Decision;
class Entity;

class StoryDecisionEvent : public Event {
private:
    std::string storyId;
    std::string storyText;
    std::vector<std::shared_ptr<Decision>> availableDecisions;
    Entity* triggeringEntity; // Слабая ссылка

public:
    // Конструктор
    StoryDecisionEvent(const std::string& storyId, const std::string& storyText,
        Entity* triggeringEntity = nullptr);

    // Переопределение виртуальных методов
    void execute() override;
    std::string getDescription() const override;

    // Управление решениями
    void addDecision(std::shared_ptr<Decision> decision);
    void removeDecision(const std::shared_ptr<Decision>& decision);
    const std::vector<std::shared_ptr<Decision>>& getAvailableDecisions() const;

    // Геттеры
    const std::string& getStoryId() const;
    const std::string& getStoryText() const;
    Entity* getTriggeringEntity() const;

    // Сеттеры
    void setStoryText(const std::string& text);
    void setTriggeringEntity(Entity* entity);
};

#endif // STORYDECISIONEVENT_H