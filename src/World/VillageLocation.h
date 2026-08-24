// World/VillageLocation.h
#ifndef VILLAGELOCATION_H
#define VILLAGELOCATION_H

#include "Location.h"
#include <string>
#include <memory>

// Forward declarations
class GameWorld;
class Entity;

class VillageLocation : public Location {
private:
    int population;
    std::string villageType;

public:
    // Конструктор
    VillageLocation(const std::string& name, const std::string& description,
        GameWorld* world, const std::string& villageType = "Обычная");

    // Переопределение виртуальных методов
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;

    // Геттеры/сеттеры
    int getPopulation() const;
    void setPopulation(int population);
    const std::string& getVillageType() const;
    void setVillageType(const std::string& type);

    // Специфичные методы для деревни
    void addVillager(std::shared_ptr<Entity> villager);
    void removeVillager(const std::shared_ptr<Entity>& villager);
};

#endif // VILLAGELOCATION_H