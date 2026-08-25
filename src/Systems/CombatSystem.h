// Systems/CombatSystem.h
#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include <vector>
#include <memory>
#include <string>

// Forward declarations
class Entity;
class CombatEvent;

class CombatSystem {
private:
    std::vector<std::shared_ptr<CombatEvent>> combatEvents;

public:
    // Конструктор/деструктор
    CombatSystem();
    ~CombatSystem() = default;

    // Методы боя
    void initiateCombat(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender);
    void processCombatRound();
    bool isCombatActive() const;
    void update(float deltaTime); // Добавлено объявление

    // Управление событиями боя
    void addCombatEvent(std::shared_ptr<CombatEvent> event);
    void removeCombatEvent(const std::shared_ptr<CombatEvent>& event);
    const std::vector<std::shared_ptr<CombatEvent>>& getCombatEvents() const;

    // Статус боя
    void displayCombatStatus() const;

    // Запрет копирования
    CombatSystem(const CombatSystem&) = delete;
    CombatSystem& operator=(const CombatSystem&) = delete;
};

#endif // COMBATSYSTEM_H