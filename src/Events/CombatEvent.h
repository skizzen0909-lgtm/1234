// Events/CombatEvent.h
#ifndef COMBATEVENT_H
#define COMBATEVENT_H

#include "Event.h"
#include <memory>

// Forward declarations
class Entity;

class CombatEvent : public Event {
private:
    std::shared_ptr<Entity> attacker;
    std::shared_ptr<Entity> defender;
    int damage;

public:
    // Конструктор
    CombatEvent(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender, int damage);

    // Реализация виртуальных методов
    void execute() override;
    std::string getDescription() const override;

    // Геттеры
    std::shared_ptr<Entity> getAttacker() const;
    std::shared_ptr<Entity> getDefender() const;
    int getDamage() const;
};

#endif // COMBATEVENT_H