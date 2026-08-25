// Entities/VillagerNPC.h
#ifndef VILLAGERNPC_H
#define VILLAGERNPC_H

#include "NPC.h"
#include <string>
#include <memory>

// Forward declarations
class Entity;
class Quest;

class VillagerNPC : public NPC {
private:
    std::string occupation;
    std::vector<std::shared_ptr<Quest>> offeredQuests;

public:
    // Конструктор
    VillagerNPC(const std::string& name, const std::string& occupation, int health = 50);

    // Переопределение виртуальных методов
    std::string getType() const override;
    void interact(Entity* player) override;
    std::string getDialogue() const override;

    // Геттеры
    const std::string& getOccupation() const;
    const std::vector<std::shared_ptr<Quest>>& getOfferedQuests() const;

    // Сеттеры
    void setOccupation(const std::string& occupation);

    // Управление квестами
    void addQuest(std::shared_ptr<Quest> quest);
    void removeQuest(const std::shared_ptr<Quest>& quest);

    // Запрет копирования (наследуется от NPC)
    std::pair<int, int> decideMovement() override;
};

#endif // VILLAGERNPC_H