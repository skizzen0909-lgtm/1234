// World/Ecosystem.cpp
#include "Ecosystem.h"
#include "TileBasedLocation.h"
#include "Entities/Enemy.h"
#include "Entities/VillagerNPC.h"
#include <random>
#include "World/TileMap.h" // Добавлено: включаем TileMap.h

Ecosystem::Ecosystem() {
    initializeDefaultSpecies();
}

void Ecosystem::initializeDefaultSpecies() {
    speciesList = {
        {"Deer", "animal", 5, "Forest"},
        {"Wolf", "animal", 3, "Forest"},
        {"Bear", "animal", 1, "Forest"},
        {"Oak Tree", "plant", 10, "Forest"},
        {"Cactus", "plant", 8, "Desert"},
        {"Scorpion", "monster", 4, "Desert"},
        {"Fish", "animal", 6, "Ocean"},
        {"Shark", "monster", 2, "Ocean"},
        {"Mountain Goat", "animal", 3, "Mountain"},
        {"Yeti", "monster", 1, "Mountain"}
    };
}

std::vector<std::shared_ptr<Entity>> Ecosystem::generateFlora(const std::string& biome, int count) {
    std::vector<std::shared_ptr<Entity>> flora;
    // Placeholder: In real implementation, create plant entities or decorative tiles
    return flora;
}

std::vector<std::shared_ptr<Entity>> Ecosystem::generateFauna(const std::string& biome, int count) {
    std::vector<std::shared_ptr<Entity>> fauna;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> healthDist(20, 80);
    std::uniform_int_distribution<> attackDist(5, 20);
    for (int i = 0; i < count; ++i) {
        std::string name = "Wild " + biome + " Creature " + std::to_string(i);
        int health = healthDist(gen);
        int attack = attackDist(gen);
        auto creature = std::make_shared<Enemy>(name, "wildlife", health, attack);
        fauna.push_back(creature);
    }
    return fauna;
}

void Ecosystem::populateLocation(TileBasedLocation* location, const std::string& biomeName) {
    if (!location) return;
    int floraCount = 10;
    int faunaCount = 5;
    auto flora = generateFlora(biomeName, floraCount);
    auto fauna = generateFauna(biomeName, faunaCount);
    for (auto& entity : flora) {
        // In real implementation, add to tile map as decoration or static entity
    }
    for (auto& entity : fauna) {
        auto pos = location->getTileMap()->findEmptyWalkableTile();
        if (pos.first != -1 && pos.second != -1) {
            entity->setPosition(pos.first, pos.second); // Исправлено: передаем два аргумента
            location->addEntity(entity);
        }
    }
}