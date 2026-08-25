// AI/GoalSystem.h
#pragma once
#include <vector>
#include <string>
#include <functional>
#include <memory>

class Entity;

struct Goal {
    std::string id;
    std::string description;
    float priority = 0.0f;           
    std::function<bool(Entity*)> condition;
    std::function<void(Entity*)> action;
    bool isCompleted = false;        
};

class GoalSystem {
private:
    std::vector<Goal> activeGoals;
    std::vector<Goal> completedGoals;

public:
    GoalSystem();

    void addGoal(const Goal& goal);
    void update(Entity* owner, float deltaTime);
    Goal* getCurrentGoal();
    void completeGoal(const std::string& goalId);
};