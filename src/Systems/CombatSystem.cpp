// Systems/CombatSystem.cpp
#include "CombatSystem.h"
#include "Entities/Entity.h"
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Events/CombatEvent.h"
#include "Events/EventManager.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <memory>
CombatSystem::CombatSystem() {
}

void CombatSystem::initiateCombat(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender) {
    if (!attacker || !defender || !attacker->isAlive() || !defender->isAlive()) {
        return;
    }

    std::cout << "Бой начинается между " << attacker->getName() << " и " << defender->getName() << "!\n";

    // Создаем начальное событие боя
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> damageDist(5, 15);

    int damage = damageDist(gen);
    auto combatEvent = std::make_shared<CombatEvent>(attacker, defender, damage);
    addCombatEvent(combatEvent);
}

void CombatSystem::processCombatRound() {
    if (combatEvents.empty()) return;

    for (const auto& event : combatEvents) {
        if (event && !event->isHandled()) {
            event->execute();

            // Проверяем смерть защитника
            auto defender = event->getDefender();
            if (defender && !defender->isAlive()) {
                if (auto enemy = std::dynamic_pointer_cast<Enemy>(defender)) {
                    if (auto attacker = event->getAttacker()) {
                        if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
                            player->gainExperience(enemy->getLootExperience());
                        }
                    }
                }
            }
        }
    }

    // Очищаем обработанные события
    combatEvents.clear();
}

bool CombatSystem::isCombatActive() const {
    return !combatEvents.empty();
}

void CombatSystem::update(float deltaTime) {
    static float combatTimer = 0.0f;
    combatTimer += deltaTime;

    if (combatTimer >= 1.0f && isCombatActive()) {
        processCombatRound();
        combatTimer = 0.0f;
    }
}

void CombatSystem::addCombatEvent(std::shared_ptr<CombatEvent> event) {
    if (event) {
        combatEvents.push_back(event);
    }
}

void CombatSystem::removeCombatEvent(const std::shared_ptr<CombatEvent>& event) {
    if (!event) return;

    auto it = std::find(combatEvents.begin(), combatEvents.end(), event);
    if (it != combatEvents.end()) {
        combatEvents.erase(it);
    }
}

const std::vector<std::shared_ptr<CombatEvent>>& CombatSystem::getCombatEvents() const {
    return combatEvents;
}

void CombatSystem::displayCombatStatus() const {
    std::cout << "\n=== Статус боя ===\n";
    for (const auto& event : combatEvents) {
        if (event) {
            std::cout << event->getDescription() << "\n";
        }
    }
    std::cout << "=================\n";
}