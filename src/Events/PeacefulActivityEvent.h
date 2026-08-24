// Events/PeacefulActivityEvent.h
#ifndef PEACEFULACTIVITYEVENT_H
#define PEACEFULACTIVITYEVENT_H

#include "Event.h"
#include <string>
#include <memory>

// Forward declarations
class Entity;

enum class ActivityType {
    TRADE,
    REST,
    CRAFT,
    TRAIN,
    SOCIALIZE,
    EXPLORE
};

class PeacefulActivityEvent : public Event {
private:
    ActivityType activityType;
    std::string activityDescription;
    int duration; // В минутах
    int experienceGained;
    Entity* participant1; // Слабая ссылка
    Entity* participant2; // Слабая ссылка (может быть nullptr)

public:
    // Конструктор
    PeacefulActivityEvent(ActivityType type, const std::string& description,
        int duration, int experienceGained = 0,
        Entity* participant1 = nullptr, Entity* participant2 = nullptr);

    // Переопределение виртуальных методов
    void execute() override;
    std::string getDescription() const override;

    // Геттеры
    ActivityType getActivityType() const;
    const std::string& getActivityDescription() const;
    int getDuration() const;
    int getExperienceGained() const;
    Entity* getParticipant1() const;
    Entity* getParticipant2() const;

    // Сеттеры
    void setActivityDescription(const std::string& description);
    void setDuration(int duration);
    void setExperienceGained(int exp);
    void setParticipant1(Entity* participant);
    void setParticipant2(Entity* participant);
};

#endif // PEACEFULACTIVITYEVENT_H