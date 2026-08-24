#pragma once

#include "GoalSystemExtended.h"
#include "EmotionalSystem.h"
#include "LearningSystem.h"
#include "MemorySystem.h"
#include <vector>
#include <string>
#include <memory>

// Перечисление возможных действий, которые может предпринять агент
enum class Action {
    IDLE,
    MOVE_TO_POINT,
    ATTACK_ENEMY,
    FLEE_FROM_ENEMY,
    TALK_TO_NPC,
    GATHER_RESOURCE,
    CRAFT_ITEM,
    REST,
    USE_ITEM,
    COMPLETE_QUEST
};

struct ActionScore {
    Action action;
    float score;
    std::string reason;
};

/**
 * BrainController - Центральный процессор принятия решений.
 * Объединяет данные от всех систем AI для выбора наилучшего действия в текущий момент.
 */
class BrainController {
public:
    BrainController();
    ~BrainController();

    // Инициализация систем
    void initialize(const std::string& agentName);

    // Основной цикл мышления: возвращает лучшее действие
    Action think(float deltaTime);
    
    // Получение объяснения последнего выбранного действия (для отладки/логов)
    std::string getLastDecisionReason() const;

    // Внешние триггеры событий
    void onEvent(const std::string& eventType, void* context = nullptr);
    
    // Доступ к подсистемам (для отладки или внешней интеграции)
    EmotionalSystem& getEmotions() { return emotions; }
    GoalSystem& getGoals() { return goals; }
    LearningSystem& getLearning() { return learning; }
    MemorySystem& getMemory() { return memory; }

private:
    std::string agentName;
    
    EmotionalSystem emotions;
    GoalSystem goals;
    LearningSystem learning;
    MemorySystem memory;

    Action lastAction;
    std::string lastDecisionReason;
    float decisionCooldown;

    // Внутренние методы оценки
    std::vector<ActionScore> evaluatePossibleActions();
    float calculateActionScore(Action action, const Goal& bestGoal);
    void applyEmotionalModifiers(Action action, float& score);
    void learnFromAction(Action action, float outcomeQuality);
};
