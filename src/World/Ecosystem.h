// World/Ecosystem.h
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Entities/Entity.h"

class GameWorld;
class TileBasedLocation;

struct Species {
    std::string name;
    std::string type; // "animal", "plant", "monster"
    int spawnWeight;
    std::string preferredBiome;
};

class Ecosystem {
private:
    std::vector<Species> speciesList;

    std::vector<std::shared_ptr<Entity>> generateFlora(const std::string& biome, int count);
    std::vector<std::shared_ptr<Entity>> generateFauna(const std::string& biome, int count);

public:
    Ecosystem();
    void populateLocation(TileBasedLocation* location, const std::string& biomeName);
    void initializeDefaultSpecies();
};