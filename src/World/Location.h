// World/Location.h
#ifndef LOCATION_H
#define LOCATION_H
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Entities/Player.h" // <-- Добавлено: включаем полное определение Player
class Location {
protected:
    std::string name;
    std::string description;
    std::vector<std::shared_ptr<Entity>> entities;
public:
    Location(const std::string& name, const std::string& description);
    virtual ~Location() = default;
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    // --- Виртуальные методы для переопределения в наследниках ---
    virtual std::shared_ptr<Player> findPlayer() const;
    virtual std::vector<std::shared_ptr<Entity>> getEntitiesInRange(int centerX, int centerY, int range) const;
    virtual bool addEntity(std::shared_ptr<Entity> entity);
    virtual void removeEntity(std::shared_ptr<Entity> entity);
    virtual void enter() {}
    virtual void exit() {}
    virtual void update(float deltaTime) {}
    // --- Методы для работы с сущностями ---
    const std::vector<std::shared_ptr<Entity>>& getEntities() const { return entities; }
    size_t getEntityCount() const { return entities.size(); }
};
#endif // LOCATION_H