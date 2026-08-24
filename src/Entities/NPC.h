// Entities/NPC.h
#ifndef NPC_H
#define NPC_H

#include "Entity.h"
#include <string>
#include <memory>

// Forward declarations
class Dialogue;

class NPC : public Entity {
protected:
    std::string npcType;
    int relationshipLevel; // Отношение к игроку

public:
    // Конструктор
    NPC(const std::string& name, const std::string& npcType, int health = 50);

    // Переопределение виртуальных методов
    std::string getType() const override;

    // Геттеры
    const std::string& getNpcType() const;
    int getRelationshipLevel() const;

    // Сеттеры
    void setNpcType(const std::string& type);
    void setRelationshipLevel(int level);

    // Методы взаимодействия
    virtual void interact(Entity* player);
    virtual std::string getDialogue() const;

    // Запрет копирования (наследуется от Entity)
};

#endif // NPC_H