// World/RandomEncounterEvent.cpp
#include "RandomEncounterEvent.h"
#include "Entities/Entity.h"
#include <iostream>

RandomEncounterEvent::RandomEncounterEvent(const std::string& id,
                                           const std::string& enemyType,
                                           int enemyLevel,
                                           float duration)
    : WorldEvent(id, 
                 "Случайная встреча: " + enemyType,
                 "Вы встретили " + enemyType + " уровня " + std::to_string(enemyLevel),
                 WorldEventType::RANDOM_ENCOUNTER,
                 duration,
                 5), // Высокий приоритет для встреч с врагами
      enemyLevel(enemyLevel),
      enemyType(enemyType),
      encounterResolved(false) {
}

void RandomEncounterEvent::handleEvent(Entity* player) {
    if (!player || encounterResolved) {
        return;
    }
    
    std::cout << "!!! " << eventName << " !!!" << std::endl;
    std::cout << description << std::endl;
    std::cout << "Приготовьтесь к бою!" << std::endl;
    
    // Здесь должна быть логика начала боя через CombatSystem
    // combatSystem->initiateCombat(player, createEnemy(enemyType, enemyLevel));
}

void RandomEncounterEvent::resolveEncounter(bool won) {
    encounterResolved = true;
    
    if (won) {
        std::cout << "Победа! Вы одолели " << enemyType << "." << std::endl;
        // Начисление опыта и лута должно происходить здесь
    } else {
        std::cout << "Вы потерпели поражение от " << enemyType << "..." << std::endl;
        // Логика поражения (отступление, потеря здоровья и т.д.)
    }
    
    deactivate();
}

bool RandomEncounterEvent::isResolved() const {
    return encounterResolved;
}

int RandomEncounterEvent::getEnemyLevel() const {
    return enemyLevel;
}

const std::string& RandomEncounterEvent::getEnemyType() const {
    return enemyType;
}
