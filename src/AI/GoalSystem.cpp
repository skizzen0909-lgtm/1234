// AI/GoalSystem.cpp
#include "GoalSystem.h"
#include "Entities/Entity.h"
#include <algorithm>


GoalSystem::GoalSystem() {}

void GoalSystem::addGoal(const Goal& goal) {
    activeGoals.push_back(goal);
}

Goal* GoalSystem::getCurrentGoal() {
    if (activeGoals.empty()) return nullptr;

    // Sort by priority
    std::sort(activeGoals.begin(), activeGoals.end(),
        [](const Goal& a, const Goal& b) {
            return a.priority > b.priority;
        });

    return &activeGoals[0];
}

void GoalSystem::update(Entity* owner, float deltaTime) {
    if (!owner) return;

    for (auto& goal : activeGoals) {
        if (!goal.isCompleted && goal.condition(owner)) {
            goal.action(owner);
            goal.isCompleted = true;
            completedGoals.push_back(goal);
        }
    }

    // Remove completed goals from active list
    activeGoals.erase(
        std::remove_if(activeGoals.begin(), activeGoals.end(),
            [](const Goal& goal) { return goal.isCompleted; }),
        activeGoals.end());
}

void GoalSystem::completeGoal(const std::string& goalId) {
    auto it = std::find_if(activeGoals.begin(), activeGoals.end(),
        [&goalId](const Goal& goal) { return goal.id == goalId; });
    if (it != activeGoals.end()) {
        it->isCompleted = true;
        completedGoals.push_back(*it);
        activeGoals.erase(it);
    }
}