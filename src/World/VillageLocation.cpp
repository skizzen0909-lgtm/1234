// World/VillageLocation.cpp
#include "VillageLocation.h"
#include "GameWorld.h"
#include "Entities/Entity.h"
#include <iostream>

VillageLocation::VillageLocation(const std::string& name, const std::string& description, GameWorld* world, const std::string& villageType)
    : Location(name, description), villageType(villageType) {
    if (!world) {
        // Обработка ошибки: world не может быть nullptr
        std::cerr << "Warning: VillageLocation created with null GameWorld pointer." << std::endl;
    }
}

void VillageLocation::enter() {
    Location::enter();
    std::cout << "Вы входите в деревню " << getName() << std::endl;
}

void VillageLocation::exit() {
    Location::exit();
    std::cout << "Вы покидаете деревню " << getName() << std::endl;
}

void VillageLocation::update(float deltaTime) {
    Location::update(deltaTime);
}

int VillageLocation::getPopulation() const {
    return population;
}

void VillageLocation::setPopulation(int population) {
    if (population >= 0) {
        this->population = population;
    }
}

const std::string& VillageLocation::getVillageType() const {
    return villageType;
}

void VillageLocation::setVillageType(const std::string& type) {
    villageType = type;
}

void VillageLocation::addVillager(std::shared_ptr<Entity> villager) {
    if (addEntity(villager)) {
        population++;
    }
}

void VillageLocation::removeVillager(const std::shared_ptr<Entity>& villager) {
    removeEntity(villager);
    if (population > 0) {
        population--;
    }
}