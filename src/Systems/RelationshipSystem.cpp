// Systems/RelationshipSystem.cpp

#include "RelationshipSystem.h"
#include <iostream>
#include <algorithm>
#include <climits>

RelationshipSystem::RelationshipSystem() {
}

void RelationshipSystem::setRelationship(const std::string& entityId1, const std::string& entityId2, int value) {
    if (entityId1 == entityId2) {
        return; // Нельзя иметь отношения с самим собой
    }

    // Ограничиваем значение диапазоном [-100, 100]
    if (value > 100) value = 100;
    if (value < -100) value = -100;

    // Устанавливаем отношения в обоих направлениях
    Relationship rel(entityId2, value);
    relationships[entityId1][entityId2] = rel;

    Relationship reverseRel(entityId1, value);
    relationships[entityId2][entityId1] = reverseRel;
}

int RelationshipSystem::getRelationshipValue(const std::string& entityId1, const std::string& entityId2) const {
    auto it1 = relationships.find(entityId1);
    if (it1 != relationships.end()) {
        auto it2 = it1->second.find(entityId2);
        if (it2 != it1->second.end()) {
            return it2->second.value;
        }
    }
    return 0; // Нейтральные отношения по умолчанию
}

RelationshipType RelationshipSystem::getRelationshipType(const std::string& entityId1, const std::string& entityId2) const {
    auto it1 = relationships.find(entityId1);
    if (it1 != relationships.end()) {
        auto it2 = it1->second.find(entityId2);
        if (it2 != it1->second.end()) {
            return it2->second.type;
        }
    }
    return RelationshipType::NEUTRAL; // Нейтральные отношения по умолчанию
}

void RelationshipSystem::modifyRelationship(const std::string& entityId1, const std::string& entityId2, int delta) {
    if (entityId1 == entityId2) {
        return;
    }

    int currentValue = getRelationshipValue(entityId1, entityId2);
    int newValue = currentValue + delta;

    // Ограничиваем значение диапазоном [-100, 100]
    if (newValue > 100) newValue = 100;
    if (newValue < -100) newValue = -100;

    setRelationship(entityId1, entityId2, newValue);
}

void RelationshipSystem::improveRelationship(const std::string& entityId1, const std::string& entityId2, int amount) {
    if (amount <= 0) return;
    modifyRelationship(entityId1, entityId2, amount);
}

void RelationshipSystem::worsenRelationship(const std::string& entityId1, const std::string& entityId2, int amount) {
    if (amount <= 0) return;
    modifyRelationship(entityId1, entityId2, -amount);
}

const std::unordered_map<std::string, Relationship>* RelationshipSystem::getEntityRelationships(const std::string& entityId) const {
    auto it = relationships.find(entityId);
    if (it != relationships.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<std::string> RelationshipSystem::getRelatedEntities(const std::string& entityId) const {
    std::vector<std::string> related;

    auto it = relationships.find(entityId);
    if (it != relationships.end()) {
        for (const auto& pair : it->second) {
            related.push_back(pair.first);
        }
    }

    return related;
}

void RelationshipSystem::removeEntity(const std::string& entityId) {
    // Удаляем все отношения, связанные с этой сущностью
    relationships.erase(entityId);

    // Удаляем упоминания этой сущности из других отношений
    for (auto& pair : relationships) {
        pair.second.erase(entityId);
    }
}

void RelationshipSystem::clearRelationships() {
    relationships.clear();
}

bool RelationshipSystem::hasRelationship(const std::string& entityId1, const std::string& entityId2) const {
    auto it1 = relationships.find(entityId1);
    if (it1 != relationships.end()) {
        return it1->second.find(entityId2) != it1->second.end();
    }
    return false;
}