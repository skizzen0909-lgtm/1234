#include "BrainController.h"
#include <algorithm>
#include <iostream>
#include <cmath>

BrainController::BrainController() 
    : lastAction(Action::IDLE), decisionCooldown(0.0f) {
}

BrainController::~BrainController() {
}

void BrainController::initialize(const std::string& name) {
    agentName = name;
    // EmotionalSystem не имеет initialize(), используем конструктор по умолчанию
    // GoalSystem не имеет initialize(), используем конструктор по умолчанию
    // LearningSystem не имеет initialize(), используем конструктор по умолчанию
    // MemorySystem не имеет initialize(), используем конструктор по умолчанию
    
    std::cout << "[BrainController] Агент '" << agentName << "' инициализирован." << std::endl;
}

Action BrainController::think(float deltaTime) {
    // Обновление таймеров
    if (decisionCooldown > 0) {
        decisionCooldown -= deltaTime;
        return lastAction; // Возвращаем предыдущее действие, если cooldown не прошел
    }
    
    // Сброс cooldown для частых решений (можно настроить)
    decisionCooldown = 0.5f; 

    // 1. Оценка всех возможных действий
    auto actionScores = evaluatePossibleActions();
    
    if (actionScores.empty()) {
        return Action::IDLE;
    }
    
    // 2. Выбор действия с наивысшим скором
    auto bestIt = std::max_element(actionScores.begin(), actionScores.end(),
        [](const ActionScore& a, const ActionScore& b) {
            return a.score < b.score;
        });
    
    lastAction = bestIt->action;
    lastDecisionReason = bestIt->reason;
    
    // 3. Обучение на основе выбранного действия (предварительная оценка)
    learnFromAction(lastAction, 0.5f); // Нейтральная оценка до выполнения
    
    std::cout << "[BrainController] Агент '" << agentName << "' выбрал: " 
              << static_cast<int>(lastAction) << " (" << lastDecisionReason << ")" << std::endl;
    
    return lastAction;
}

std::string BrainController::getLastDecisionReason() const {
    return lastDecisionReason;
}

void BrainController::onEvent(const std::string& eventType, void* context) {
    // Обработка событий через эмоциональную систему
    if (eventType == "threat") {
        emotions.onThreatDetected(0.8f);
    } else if (eventType == "success") {
        emotions.onSuccess(0.7f);
    } else if (eventType == "failure") {
        emotions.onFailure(0.6f);
    } else if (eventType == "social_positive") {
        emotions.onSocialInteraction(true, 0.5f);
    } else if (eventType == "social_negative") {
        emotions.onSocialInteraction(false, 0.5f);
    } else if (eventType == "discovery") {
        emotions.onDiscovery(0.6f);
    }
    
    // Запись в память (используем addMemory вместо addEvent)
    // memory.addEvent(eventType, 0.8f); // Метод не существует в текущей версии
    
    // Возможность создания новых целей на основе событий
    if (eventType == "threat") {
        Goal survivalGoal;
        survivalGoal.id = "survival_" + std::to_string(rand());
        survivalGoal.type = GoalType::SURVIVAL;
        survivalGoal.description = "Избежать угрозы";
        survivalGoal.priority = GoalPriority::CRITICAL;
        survivalGoal.status = GoalStatus::ACTIVE;
        goals.addGoal(survivalGoal);
    }
}

std::vector<ActionScore> BrainController::evaluatePossibleActions() {
    std::vector<ActionScore> scores;
    
    // Получаем лучшую цель для оценки приоритетов
    Goal* bestGoalPtr = goals.getCurrentGoal();
    
    // Если нет активных целей, добавляем базовые
    Goal defaultGoal;
    if (!bestGoalPtr) {
        defaultGoal.id = "idle_" + std::to_string(rand());
        defaultGoal.type = GoalType::SURVIVAL;
        defaultGoal.description = "Отдых и восстановление";
        defaultGoal.priority = GoalPriority::LOW;
        goals.addGoal(defaultGoal);
        bestGoalPtr = goals.getCurrentGoal();
    }
    
    if (!bestGoalPtr) {
        // Если всё ещё нет цели, возвращаем IDLE
        ActionScore idleScore;
        idleScore.action = Action::IDLE;
        idleScore.score = 1.0f;
        idleScore.reason = "Нет активных целей";
        return {idleScore};
    }
    
    const Goal& bestGoal = *bestGoalPtr;
    
    // Оцениваем каждое возможное действие
    std::vector<Action> possibleActions = {
        Action::IDLE,
        Action::MOVE_TO_POINT,
        Action::ATTACK_ENEMY,
        Action::FLEE_FROM_ENEMY,
        Action::TALK_TO_NPC,
        Action::GATHER_RESOURCE,
        Action::CRAFT_ITEM,
        Action::REST,
        Action::USE_ITEM,
        Action::COMPLETE_QUEST
    };
    
    for (const auto& action : possibleActions) {
        float score = calculateActionScore(action, bestGoal);
        applyEmotionalModifiers(action, score);
        
        ActionScore as;
        as.action = action;
        as.score = score;
        as.reason = "Действие для цели: " + std::to_string(static_cast<int>(bestGoal.type));
        
        scores.push_back(as);
    }
    
    return scores;
}

float BrainController::calculateActionScore(Action action, const Goal& goal) {
    float baseScore = 0.0f;
    
    // Базовая оценка соответствия действия текущей цели
    switch (goal.type) {
        case GoalType::SURVIVAL:
            if (action == Action::REST || action == Action::FLEE_FROM_ENEMY || action == Action::USE_ITEM) {
                baseScore = 0.9f;
            } else if (action == Action::ATTACK_ENEMY) {
                baseScore = 0.7f; // Атака как защита
            }
            break;
            
        case GoalType::COMBAT:
            if (action == Action::ATTACK_ENEMY) {
                baseScore = 1.0f;
            } else if (action == Action::FLEE_FROM_ENEMY) {
                baseScore = 0.6f; // Отступление если здоровье низкое
            } else if (action == Action::USE_ITEM) {
                baseScore = 0.8f; // Использование зелий
            }
            break;
            
        case GoalType::EXPLORATION:
            if (action == Action::MOVE_TO_POINT) {
                baseScore = 0.95f;
            } else if (action == Action::GATHER_RESOURCE) {
                baseScore = 0.7f; // Попутный сбор
            }
            break;
            
        case GoalType::SOCIAL:
            if (action == Action::TALK_TO_NPC) {
                baseScore = 1.0f;
            }
            break;
            
        case GoalType::GATHERING:
            if (action == Action::GATHER_RESOURCE) {
                baseScore = 1.0f;
            } else if (action == Action::MOVE_TO_POINT) {
                baseScore = 0.6f; // Движение к ресурсу
            }
            break;
            
        case GoalType::CRAFTING:
            if (action == Action::CRAFT_ITEM) {
                baseScore = 1.0f;
            } else if (action == Action::GATHER_RESOURCE) {
                baseScore = 0.8f; // Сбор материалов
            }
            break;
            
        case GoalType::TRADING:
            if (action == Action::TALK_TO_NPC) {
                baseScore = 0.9f;
            } else if (action == Action::MOVE_TO_POINT) {
                baseScore = 0.7f; // Движение к торговцу
            }
            break;
            
        case GoalType::QUEST:
            if (action == Action::COMPLETE_QUEST || action == Action::MOVE_TO_POINT) {
                baseScore = 0.95f;
            } else if (action == Action::ATTACK_ENEMY) {
                baseScore = 0.7f; // Устранение препятствий
            }
            break;
            
        default:
            baseScore = 0.3f;
            break;
    }
    
    // Модификатор от обучения (используем getBestAction вместо getActionWeight)
    std::string situationType = "combat"; // Можно определить динамически
    std::string bestActionStr = learning.getBestAction(situationType);
    // Простая эвристика: если есть успешное действие для ситуации, добавляем бонус
    float learningBonus = !bestActionStr.empty() ? 0.1f : 0.0f;
    baseScore += learningBonus;
    
    return baseScore;
}

void BrainController::applyEmotionalModifiers(Action action, float& score) {
    // Страх увеличивает шанс бегства
    if (emotions.isAfraid()) {
        if (action == Action::FLEE_FROM_ENEMY) {
            score *= 1.5f;
        } else if (action == Action::ATTACK_ENEMY) {
            score *= 0.5f;
        }
    }
    
    // Гнев увеличивает агрессию
    if (emotions.isAngry()) {
        if (action == Action::ATTACK_ENEMY) {
            score *= 1.4f;
        }
    }
    
    // Счастье улучшает социальные взаимодействия
    if (emotions.isHappy()) {
        if (action == Action::TALK_TO_NPC) {
            score *= 1.3f;
        }
    }
    
    // Стресс снижает эффективность сложных действий
    if (emotions.isStressed()) {
        if (action == Action::CRAFT_ITEM || action == Action::COMPLETE_QUEST) {
            score *= 0.7f;
        }
        // Но увеличивает желание отдохнуть
        if (action == Action::REST) {
            score *= 1.4f;
        }
    }
    
    // Проверка возможности действовать
    if (!emotions.canAct()) {
        if (action != Action::REST && action != Action::IDLE) {
            score *= 0.3f; // Сильное снижение если не может действовать
        }
    }
}

void BrainController::learnFromAction(Action action, float outcomeQuality) {
    int actionId = static_cast<int>(action);
    std::string situationType = "combat"; // Можно определить динамически
    std::string actionStr = "action_" + std::to_string(actionId);
    
    // Используем существующий метод addExperience
    learning.addExperience(situationType, actionStr, outcomeQuality);
}
