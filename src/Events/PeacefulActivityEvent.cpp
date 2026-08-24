// Events/PeacefulActivityEvent.cpp
#include "PeacefulActivityEvent.h"
#include "../Entities/Entity.h"
#include <iostream>

PeacefulActivityEvent::PeacefulActivityEvent(ActivityType type, const std::string& description,
    int duration, int experienceGained,
    Entity* participant1, Entity* participant2)
    : Event("peaceful_activity"), activityType(type), activityDescription(description),
    duration(duration), experienceGained(experienceGained),
    participant1(participant1), participant2(participant2) {
    if (duration <= 0) {
        throw std::invalid_argument("Duration must be positive");
    }
}

void PeacefulActivityEvent::execute() {
    if (isHandled()) {
        return;
    }

    std::cout << "\n=== Мирная активность ===" << std::endl;

    // Выводим информацию об активности
    std::cout << activityDescription << std::endl;
    std::cout << "Продолжительность: " << duration << " минут" << std::endl;

    if (experienceGained > 0) {
        std::cout << "Получено опыта: " << experienceGained << std::endl;
    }

    // Применяем эффекты активности участникам
    if (participant1) {
        std::cout << participant1->getName() << " участвует в активности." << std::endl;
        if (experienceGained > 0) {
            // participant1->gainExperience(experienceGained); // Предполагаем, что есть такой метод
        }
    }

    if (participant2) {
        std::cout << participant2->getName() << " участвует в активности." << std::endl;
        if (experienceGained > 0) {
            // participant2->gainExperience(experienceGained); // Предполагаем, что есть такой метод
        }
    }

    setHandled(true);
}

std::string PeacefulActivityEvent::getDescription() const {
    std::string typeStr;
    switch (activityType) {
    case ActivityType::TRADE: typeStr = "торговля"; break;
    case ActivityType::REST: typeStr = "отдых"; break;
    case ActivityType::CRAFT: typeStr = "ремесло"; break;
    case ActivityType::TRAIN: typeStr = "тренировка"; break;
    case ActivityType::SOCIALIZE: typeStr = "общение"; break;
    case ActivityType::EXPLORE: typeStr = "исследование"; break;
    }

    return "Мирная активность (" + typeStr + "): " + activityDescription;
}

ActivityType PeacefulActivityEvent::getActivityType() const {
    return activityType;
}

const std::string& PeacefulActivityEvent::getActivityDescription() const {
    return activityDescription;
}

int PeacefulActivityEvent::getDuration() const {
    return duration;
}

int PeacefulActivityEvent::getExperienceGained() const {
    return experienceGained;
}

Entity* PeacefulActivityEvent::getParticipant1() const {
    return participant1;
}

Entity* PeacefulActivityEvent::getParticipant2() const {
    return participant2;
}

void PeacefulActivityEvent::setActivityDescription(const std::string& description) {
    activityDescription = description;
}

void PeacefulActivityEvent::setDuration(int duration) {
    if (duration > 0) {
        this->duration = duration;
    }
}

void PeacefulActivityEvent::setExperienceGained(int exp) {
    if (exp >= 0) {
        experienceGained = exp;
    }
}

void PeacefulActivityEvent::setParticipant1(Entity* participant) {
    participant1 = participant;
}

void PeacefulActivityEvent::setParticipant2(Entity* participant) {
    participant2 = participant;
}