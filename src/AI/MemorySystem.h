// AI/MemorySystem.h

#ifndef MEMORYSYSTEM_H
#define MEMORYSYSTEM_H

#include <vector>
#include <memory>
#include <chrono>
#include <string>

class Entity; // Forward declaration

struct MemoryEvent {
    std::string eventType;
    std::weak_ptr<Entity> entity;
    std::chrono::steady_clock::time_point timestamp;
    float importance;
    std::string description; // Добавьте это поле

    MemoryEvent(const std::string& type, std::shared_ptr<Entity> ent,
        float imp = 1.0f, const std::string& desc = "")
        : eventType(type), entity(ent), timestamp(std::chrono::steady_clock::now()),
        importance(imp), description(desc) {
    }
};

class MemorySystem {
private:
    std::vector<MemoryEvent> memories;
    float decayRate = 0.1f; // Скорость забывания

    // Время последней атаки
    std::chrono::steady_clock::time_point lastAttackedTime;
    std::chrono::steady_clock::time_point lastSocializedTime;

public:
    MemorySystem() = default;
    ~MemorySystem() = default;

    void addMemory(const std::string& eventType, std::shared_ptr<Entity> entity, float importance = 1.0f);
    void forgetOldMemories(); // Удаляет старые или маловажные воспоминания
    void decayMemories(float deltaTime); // Постепенно снижает важность

    // Геттеры для проверки состояния
    bool hasHostileEntities() const;
    bool hasFriendlyEntities() const; // Для простоты, любые встречи
    bool wasRecentlyAttacked() const;
    bool wasRecentlySocialized() const;
    void setLastAttackedTime();
    void setLastSocializedTime();

    const std::vector<MemoryEvent>& getMemories() const { return memories; }
};

#endif // MEMORYSYSTEM_H