// src/AI/AdvancedAI.h
#ifndef ADVANCEDAI_H
#define ADVANCEDAI_H

#include "AI/AIBase.h"
#include "AI/AIPersonality.h"
#include "AI/MemorySystem.h"
#include "AI/EmotionalSystem.h"
#include "AI/GoalSystem.h" 
#include <memory>
#include <vector>
#include <random>

class Entity;

enum class AIState {
    IDLE,
    WANDER,
    FOLLOW,
    ATTACK,
    FLEE,
    GATHER,
    SOCIALIZE,
    ALERT
};

class AdvancedAI : public AIBase {
private:
    AIPersonality personality;
    MemorySystem memorySystem;
    EmotionalSystem emotionalSystem;
    std::unique_ptr<GoalSystem> goalSystem; 

    AIState currentState;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

    std::weak_ptr<Entity> followTarget;
    std::weak_ptr<Entity> fleeTarget;
    std::weak_ptr<Entity> currentTarget;

    // Вспомогательные методы
    std::shared_ptr<Entity> findHostileTarget() const;
    std::shared_ptr<Entity> findFriendlyTarget() const;

    // Дерево решений
    void assessThreats();
    void assessOpportunities();
    void makeDecision(Entity* entity);
   

    // Логика движения
    std::pair<int, int> wanderLogic(Entity* entity);
    std::pair<int, int> followLogic(Entity* entity);
    std::pair<int, int> fleeLogic(Entity* entity);
    std::pair<int, int> attackMovementLogic(Entity* entity);

    // Выполнение действий
    void executeSocialize(Entity* entity);
    void checkForLoot(Entity* entity);
    void executeWander(Entity* entity);
    void executeFollow(Entity* entity);
    void executeFlee(Entity* entity);
    void executeAttack(Entity* entity);
    void executeIdle(Entity* entity);
    void executeAlert(Entity* entity);

public:
    explicit AdvancedAI(const AIPersonality& personality);
    ~AdvancedAI() override = default;

    // Геттеры
    const AIPersonality& getPersonality() const;
    AIState getCurrentState() const;
    MemorySystem& getMemorySystem();
    const MemorySystem& getMemorySystem() const;
    EmotionalSystem& getEmotionalSystem();
    const EmotionalSystem& getEmotionalSystem() const;
    GoalSystem* getGoalSystem();
    // Сеттеры
    void setPersonality(const AIPersonality& newPersonality);
    void setFollowTarget(const std::shared_ptr<Entity>& target);
    void setFleeTarget(const std::shared_ptr<Entity>& target);
    void setCurrentTarget(const std::shared_ptr<Entity>& target);
    void setState(AIState newState);
    // Основные методы
    void update(float deltaTime, Entity* entity) override;
    void onEntityEncounter(Entity* self, const std::shared_ptr<Entity>& other) override;
    std::pair<int, int> decideMovement(Entity* entity);
};

#endif // ADVANCEDAI_H