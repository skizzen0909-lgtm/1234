// Events/CombatEvent.cpp

#include "AI/AdvancedAI.h"
#include "CombatEvent.h"
#include "Entities/Entity.h"
#include "Events/EventManager.h"
#include <sstream>
#include <stdexcept>
#include <memory>
CombatEvent::CombatEvent(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender, int damage)
    : Event("combat"), attacker(attacker), defender(defender), damage(damage) {
    if (!attacker) {
        throw std::invalid_argument("Attacker cannot be null");
    }
    if (!defender) {
        throw std::invalid_argument("Defender cannot be null");
    }
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
}

void CombatEvent::execute() {
    if (isHandled() || !attacker || !defender) {
        return;
    }

    defender->takeDamage(damage);

    // Дополнительно: отправим событие "attacked_by" в память защитника
    // Это нужно для работы hasHostileEntities() и wasRecentlyAttacked()
    if (auto aiBase = defender->getAI()) {
        if (auto advancedAI = dynamic_cast<AdvancedAI*>(aiBase)) {
            advancedAI->getMemorySystem().setLastAttackedTime();
            advancedAI->getMemorySystem().addMemory("attacked_by", attacker, 1.0f);
            advancedAI->getEmotionalSystem().addMoodModifier(-0.3f);
        }
    }

    setHandled(true);
}

std::string CombatEvent::getDescription() const {
    if (!attacker || !defender) {
        return "Invalid combat event";
    }

    std::ostringstream oss;
    oss << attacker->getName() << " attacks " << defender->getName()
        << " for " << damage << " damage";
    return oss.str();
}

std::shared_ptr<Entity> CombatEvent::getAttacker() const {
    return attacker;
}

std::shared_ptr<Entity> CombatEvent::getDefender() const {
    return defender;
}

int CombatEvent::getDamage() const {
    return damage;
}