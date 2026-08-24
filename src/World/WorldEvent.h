// World/WorldEvent.h
#ifndef WORLDEVENT_H
#define WORLDEVENT_H

#include <string>
#include <memory>
#include <functional>
#include <chrono>

class Entity;

enum class WorldEventType {
    RANDOM_ENCOUNTER,     // Случайная встреча с врагом или NPC
    TREASURE_FOUND,       // Найден клад
    WEATHER_CHANGE,       // Изменение погоды
    SPECIAL_LOCATION,     // Обнаружена особая локация
    MERCHANT_ARRIVAL,     // Прибыл торговец
    FESTIVAL,             // Праздник в деревне
    DANGER_ZONE,          // Опасная зона
    REST_AREA,            // Зона отдыха
    MYSTERIOUS_EVENT      // Таинственное событие
};

class WorldEvent {
protected:
    std::string eventId;
    std::string eventName;
    std::string description;
    WorldEventType type;
    bool active;
    float duration;           // Длительность события в секундах
    std::chrono::steady_clock::time_point startTime;
    int priority;             // Приоритет события (чем выше, тем важнее)
    
public:
    // Конструктор
    WorldEvent(const std::string& id,
               const std::string& name,
               const std::string& desc,
               WorldEventType type,
               float duration = 60.0f,
               int priority = 1);
    
    virtual ~WorldEvent() = default;
    
    // Активация события
    virtual void activate();
    
    // Деактивация события
    virtual void deactivate();
    
    // Обновление события
    virtual void update(float deltaTime);
    
    // Проверка активности
    bool isActive() const;
    
    // Проверка истечения времени
    bool isExpired() const;
    
    // Обработка события (переопределяется в наследниках)
    virtual void handleEvent(Entity* player) = 0;
    
    // Получение описания
    virtual std::string getEventDescription() const;
    
    // Геттеры
    const std::string& getEventId() const;
    const std::string& getEventName() const;
    WorldEventType getEventType() const;
    float getDuration() const;
    float getRemainingTime() const;
    int getPriority() const;
    
    // Запрет копирования
    WorldEvent(const WorldEvent&) = delete;
    WorldEvent& operator=(const WorldEvent&) = delete;
};

// Фабрика для создания событий
class WorldEventFactory {
public:
    using EventCreator = std::function<std::shared_ptr<WorldEvent>()>;
    
    static WorldEventFactory& getInstance();
    
    void registerEvent(WorldEventType type, EventCreator creator);
    std::shared_ptr<WorldEvent> createEvent(WorldEventType type);
    
private:
    WorldEventFactory() = default;
    std::unordered_map<WorldEventType, EventCreator> creators;
};

#endif // WORLDEVENT_H
