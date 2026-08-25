// src/AI/AIBase.h
#ifndef AIBASE_H
#define AIBASE_H
#include <memory>
class Entity;
class AIBase {
public:
    AIBase() = default;
    virtual ~AIBase() = default;
    virtual void update(float deltaTime, Entity* entity) = 0;
    virtual void onEntityEncounter(Entity* self, const std::shared_ptr<Entity>& other) {}
    virtual void init() {} // Добавлено: виртуальный метод для инициализации
    AIBase(const AIBase&) = delete;
    AIBase& operator=(const AIBase&) = delete;
};
#endif // AIBASE_H