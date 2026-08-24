// AI/GoalSystem.h - Расширенная версия
#pragma once
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>

class Entity;

// Приоритет цели
enum class GoalPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    CRITICAL = 3
};

// Статус цели
enum class GoalStatus {
    INACTIVE,
    ACTIVE,
    COMPLETED,
    FAILED,
    PAUSED
};

// Типы целей
enum class GoalType {
    SURVIVAL,       // Выживание (еда, отдых)
    EXPLORATION,    // Исследование
    COMBAT,         // Бой
    SOCIAL,         // Социальное взаимодействие
    GATHERING,      // Сбор ресурсов
    CRAFTING,       // Крафтинг
    TRADING,        // Торговля
    QUEST,          // Квест
    PERSONAL        // Личная цель (на основе личности)
};

struct Goal {
    std::string id;
    std::string description;
    GoalType type = GoalType::PERSONAL;
    GoalPriority priority = GoalPriority::MEDIUM;
    GoalStatus status = GoalStatus::INACTIVE;
    
    float urgency = 0.0f;              // Срочность (0.0 - 1.0)
    float importance = 0.5f;           // Важность (0.0 - 1.0)
    float timeLimit = 0.0f;            // Ограничение по времени (0 = нет)
    float elapsedTime = 0.0f;          // Прошедшее время
    
    std::function<bool(Entity*)> condition;        // Условие активации
    std::function<bool(Entity*)> completionCondition; // Условие завершения
    std::function<void(Entity*)> action;           // Действие
    std::function<void(Entity*)> onBegin;          // Callback при начале
    std::function<void(Entity*)> onComplete;       // Callback при завершении
    std::function<void(Entity*)> onFail;           // Callback при провале
    
    std::vector<std::string> prerequisites;  // Предварительные условия (ID других целей)
    std::vector<std::string> subGoals;       // Подцели
    std::string parentGoalId;                // Родительская цель
    
    int maxAttempts = 0;                     // Максимум попыток (0 = безлимита)
    int currentAttempts = 0;                 // Текущее количество попыток
    
    bool isCompleted = false;
    bool isRepeatable = false;               // Можно ли повторить
    
    Goal() = default;
    
    Goal(const std::string& goalId, const std::string& desc, GoalType gType)
        : id(goalId), description(desc), type(gType) {}
};

// Система управления целями с приоритетами
class GoalSystem {
private:
    std::vector<Goal> allGoals;
    std::priority_queue<std::pair<float, size_t>> activeGoalQueue; // Приоритет, индекс
    std::vector<Goal> completedGoals;
    std::vector<Goal> failedGoals;
    
    std::unordered_map<std::string, size_t> goalIndexMap; // ID -> индекс
    std::unordered_map<GoalType, float> typeWeights;      // Веса типов целей
    
    float globalUrgencyModifier = 1.0f;  // Глобальный модификатор срочности
    int maxActiveGoals = 5;              // Максимум активных целей
    
    // Внутренние методы
    float calculateGoalPriority(const Goal& goal, Entity* owner) const;
    void updateGoalStatus(Goal& goal, Entity* owner);
    void checkPrerequisites(Goal& goal);
    void activateNextGoal(Entity* owner);
    
public:
    GoalSystem();
    ~GoalSystem() = default;
    
    // Добавление/удаление целей
    void addGoal(const Goal& goal);
    void removeGoal(const std::string& goalId);
    bool hasGoal(const std::string& goalId) const;
    
    // Обновление системы
    void update(Entity* owner, float deltaTime);
    void recalculatePriorities(Entity* owner);
    
    // Получение целей
    Goal* getCurrentGoal();
    Goal* getGoalById(const std::string& goalId);
    const Goal* getGoalById(const std::string& goalId) const;
    std::vector<Goal*> getGoalsByType(GoalType type);
    std::vector<Goal*> getActiveGoals();
    std::vector<Goal*> getAvailableGoals(Entity* owner);
    
    // Управление статусом
    void activateGoal(const std::string& goalId);
    void pauseGoal(const std::string& goalId);
    void resumeGoal(const std::string& goalId);
    void completeGoal(const std::string& goalId);
    void failGoal(const std::string& goalId, const std::string& reason = "");
    
    // Настройка весов
    void setTypeWeight(GoalType type, float weight);
    float getTypeWeight(GoalType type) const;
    void setGlobalUrgencyModifier(float modifier);
    
    // Статистика
    int getTotalGoals() const;
    int getActiveGoalsCount() const;
    int getCompletedGoalsCount() const;
    int getFailedGoalsCount() const;
    
    // Очистка
    void clearCompletedGoals();
    void reset();
};
