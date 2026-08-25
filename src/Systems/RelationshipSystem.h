// Systems/RelationshipSystem.h
#ifndef RELATIONSHIPSYSTEM_H
#define RELATIONSHIPSYSTEM_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

// Forward declarations
class Entity;

enum class RelationshipType {
    NEUTRAL,
    FRIENDLY,
    HOSTILE,
    ALLY,
    ENEMY
};

struct Relationship {
    std::string entityId;
    int value; // От -100 (враждебность) до 100 (дружелюбие)
    RelationshipType type;

    Relationship() : value(0), type(RelationshipType::NEUTRAL) {}

    Relationship(const std::string& entityId, int value = 0)
        : entityId(entityId), value(value) {
        updateType();
    }

    void updateType() {
        if (value >= 70) {
            type = RelationshipType::ALLY;
        }
        else if (value >= 30) {
            type = RelationshipType::FRIENDLY;
        }
        else if (value <= -70) {
            type = RelationshipType::ENEMY;
        }
        else if (value <= -30) {
            type = RelationshipType::HOSTILE;
        }
        else {
            type = RelationshipType::NEUTRAL;
        }
    }
};

class RelationshipSystem {
private:
    // Карта отношений: entityId -> карта отношений с другими entity
    std::unordered_map<std::string, std::unordered_map<std::string, Relationship>> relationships;

public:
    // Конструктор/деструктор
    RelationshipSystem();
    ~RelationshipSystem() = default;

    // Управление отношениями
    void setRelationship(const std::string& entityId1, const std::string& entityId2, int value);
    int getRelationshipValue(const std::string& entityId1, const std::string& entityId2) const;
    RelationshipType getRelationshipType(const std::string& entityId1, const std::string& entityId2) const;

    // Модификация отношений
    void modifyRelationship(const std::string& entityId1, const std::string& entityId2, int delta);
    void improveRelationship(const std::string& entityId1, const std::string& entityId2, int amount);
    void worsenRelationship(const std::string& entityId1, const std::string& entityId2, int amount);

    // Получение информации
    const std::unordered_map<std::string, Relationship>* getEntityRelationships(const std::string& entityId) const;
    std::vector<std::string> getRelatedEntities(const std::string& entityId) const;

    // Утилиты
    void removeEntity(const std::string& entityId);
    void clearRelationships();
    bool hasRelationship(const std::string& entityId1, const std::string& entityId2) const;

    // Запрет копирования
    RelationshipSystem(const RelationshipSystem&) = delete;
    RelationshipSystem& operator=(const RelationshipSystem&) = delete;
};

#endif // RELATIONSHIPSYSTEM_H