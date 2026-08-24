// AI/MemorySystem.cpp

#include "MemorySystem.h"
#include "Entities/Entity.h"
#include <algorithm>
#include <chrono>
#include <memory>

void MemorySystem::addMemory(const std::string& eventType, std::shared_ptr<Entity> entity, float importance) {
    memories.emplace_back(eventType, entity, importance);
}

void MemorySystem::forgetOldMemories() {
    auto now = std::chrono::steady_clock::now();
    memories.erase(
        std::remove_if(memories.begin(), memories.end(),
            [now](const MemoryEvent& event) {
                auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - event.timestamp);
                // Забываем через 30 минут или если важность упала до 0
                return duration.count() > 30 || event.importance <= 0.0f;
            }),
        memories.end());
}

void MemorySystem::decayMemories(float deltaTime) {
    for (auto& memory : memories) {
        memory.importance -= decayRate * deltaTime;
        if (memory.importance < 0.0f) {
            memory.importance = 0.0f;
        }
    }
    // После распада забываем старое
    forgetOldMemories();
}

bool MemorySystem::hasHostileEntities() const {
    // Для примера, считаем, что если есть память об атаке, то есть враги
    // В реальной системе нужно проверять тип сущности
    for (const auto& memory : memories) {
        if (memory.eventType == "attacked_by" && memory.entity.lock()) {
            return true;
        }
    }
    return false;
}

bool MemorySystem::hasFriendlyEntities() const {
    // Для примера, любые воспоминания о встречах
    for (const auto& memory : memories) {
        if ((memory.eventType == "seen" || memory.eventType == "socialized_with") && memory.entity.lock()) {
            return true;
        }
    }
    return false;
}

bool MemorySystem::wasRecentlyAttacked() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastAttackedTime);
    return duration.count() < 10; // Был атакован менее 10 секунд назад
}

bool MemorySystem::wasRecentlySocialized() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastSocializedTime);
    return duration.count() < 30; // Был в общении менее 30 секунд назад
}

void MemorySystem::setLastAttackedTime() {
    lastAttackedTime = std::chrono::steady_clock::now();
}

void MemorySystem::setLastSocializedTime() {
    lastSocializedTime = std::chrono::steady_clock::now();
}