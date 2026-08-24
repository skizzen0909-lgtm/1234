// World/EventManager.h
#ifndef WORLDEVENTMANAGER_H
#define WORLDEVENTMANAGER_H

#include "WorldEvent.h"
#include <vector>
#include <memory>
#include <queue>
#include <random>

class Entity;

class WorldEventManager {
private:
    std::vector<std::shared_ptr<WorldEvent>> activeEvents;
    std::vector<std::shared_ptr<WorldEvent>> eventHistory;
    std::mt19937 rng;
    
    // Вероятности событий (настраиваемые)
    float baseEncounterChance;
    float treasureChance;
    float specialLocationChance;
    
    // Кулдауны для предотвращения спама событий
    float timeSinceLastEvent;
    float minEventInterval;
    
    void sortEventsByPriority();
    
public:
    WorldEventManager(unsigned int seed = 0);
    ~WorldEventManager() = default;
    
    // Обновление менеджера событий
    void update(float deltaTime, Entity* player);
    
    // Добавление события вручную
    void addEvent(std::shared_ptr<WorldEvent> event);
    
    // Генерация случайного события
    void tryGenerateRandomEvent(Entity* player);
    
    // Получение активных событий
    const std::vector<std::shared_ptr<WorldEvent>>& getActiveEvents() const;
    const std::vector<std::shared_ptr<WorldEvent>>& getEventHistory() const;
    
    // Настройка вероятностей
    void setEncounterChance(float chance);
    void setTreasureChance(float chance);
    void setSpecialLocationChance(float chance);
    void setMinEventInterval(float interval);
    
    // Очистка завершенных событий
    void cleanupFinishedEvents();
    
    // Запрет копирования
    WorldEventManager(const WorldEventManager&) = delete;
    WorldEventManager& operator=(const WorldEventManager&) = delete;
};

#endif // WORLDEVENTMANAGER_H
