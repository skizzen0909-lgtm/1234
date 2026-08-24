// World/EventManager.cpp
#include "EventManager.h"
#include "RandomEncounterEvent.h"
#include "Entities/Entity.h"
#include <algorithm>
#include <iostream>

WorldEventManager::WorldEventManager(unsigned int seed)
    : baseEncounterChance(0.15f),      // 15% шанс встречи
      treasureChance(0.08f),           // 8% шанс клада
      specialLocationChance(0.05f),    // 5% шанс особой локации
      timeSinceLastEvent(0.0f),
      minEventInterval(30.0f) {        // Минимум 30 секунд между событиями
    rng.seed(seed);
}

void WorldEventManager::update(float deltaTime, Entity* player) {
    if (!player) return;
    
    // Обновление времени с последнего события
    timeSinceLastEvent += deltaTime;
    
    // Обновление активных событий
    for (auto& event : activeEvents) {
        if (event && event->isActive()) {
            event->update(deltaTime);
            
            // Обработка события если оно активно и еще не обработано
            // (логика обработки зависит от типа события)
        }
    }
    
    // Попытка генерации случайного события
    tryGenerateRandomEvent(player);
    
    // Очистка завершенных событий
    cleanupFinishedEvents();
}

void WorldEventManager::addEvent(std::shared_ptr<WorldEvent> event) {
    if (!event) return;
    
    activeEvents.push_back(event);
    event->activate();
    sortEventsByPriority();
    
    std::cout << "[Событие] Началось: " << event->getEventName() << std::endl;
}

void WorldEventManager::tryGenerateRandomEvent(Entity* player) {
    if (!player || timeSinceLastEvent < minEventInterval) {
        return;
    }
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float roll = dist(rng);
    
    // Определение типа события на основе вероятностей
    if (roll < baseEncounterChance) {
        // Случайная встреча
        std::vector<std::string> enemyTypes = {"Гоблин", "Волк", "Разбойник", "Слизень"};
        std::uniform_int_distribution<int> enemyDist(0, static_cast<int>(enemyTypes.size()) - 1);
        std::uniform_int_distribution<int> levelDist(1, 5);
        
        std::string enemyType = enemyTypes[enemyDist(rng)];
        int enemyLevel = levelDist(rng);
        
        auto encounterEvent = std::make_shared<RandomEncounterEvent>(
            "enc_" + std::to_string(time(nullptr)),
            enemyType,
            enemyLevel
        );
        
        addEvent(encounterEvent);
        encounterEvent->handleEvent(player);
        
        timeSinceLastEvent = 0.0f;
    }
    else if (roll < baseEncounterChance + treasureChance) {
        // Найден клад (можно расширить)
        std::cout << "[Событие] Вы нашли тайник с сокровищами!" << std::endl;
        timeSinceLastEvent = 0.0f;
    }
    else if (roll < baseEncounterChance + treasureChance + specialLocationChance) {
        // Особая локация (можно расширить)
        std::cout << "[Событие] Вы обнаружили древние руины!" << std::endl;
        timeSinceLastEvent = 0.0f;
    }
}

const std::vector<std::shared_ptr<WorldEvent>>& WorldEventManager::getActiveEvents() const {
    return activeEvents;
}

const std::vector<std::shared_ptr<WorldEvent>>& WorldEventManager::getEventHistory() const {
    return eventHistory;
}

void WorldEventManager::setEncounterChance(float chance) {
    baseEncounterChance = std::max(0.0f, std::min(1.0f, chance));
}

void WorldEventManager::setTreasureChance(float chance) {
    treasureChance = std::max(0.0f, std::min(1.0f, chance));
}

void WorldEventManager::setSpecialLocationChance(float chance) {
    specialLocationChance = std::max(0.0f, std::min(1.0f, chance));
}

void WorldEventManager::setMinEventInterval(float interval) {
    minEventInterval = std::max(0.0f, interval);
}

void WorldEventManager::cleanupFinishedEvents() {
    activeEvents.erase(
        std::remove_if(activeEvents.begin(), activeEvents.end(),
            [](const std::shared_ptr<WorldEvent>& event) {
                return event && !event->isActive();
            }),
        activeEvents.end()
    );
}

void WorldEventManager::sortEventsByPriority() {
    std::sort(activeEvents.begin(), activeEvents.end(),
        [](const std::shared_ptr<WorldEvent>& a, const std::shared_ptr<WorldEvent>& b) {
            return a->getPriority() > b->getPriority();
        });
}
