// World/BiomeSystem.h
#ifndef BIOMESYSTEM_H
#define BIOMESYSTEM_H

#include "Types/Types.h"
#include <vector>
#include <string>

struct BiomeDefinition {
    std::string name;
    TileType baseTile;
    std::vector<TileType> commonObjects;   // деревья, камни и т.д.
    std::vector<TileType> rareObjects;     // руины, сокровища
    float minHeight;
    float maxHeight;
    float minMoisture;
    float maxMoisture;
    float minTemperature;
    float maxTemperature;
};

class BiomeSystem {
private:
    std::vector<BiomeDefinition> biomes;

    BiomeDefinition determineBiome(float height, float moisture, float temperature) const;

public:
    BiomeSystem();
    void initializeDefaultBiomes();
    // --- ИСПРАВЛЕНО: убраны x, y ---
    BiomeDefinition getBiomeAt(float height, float moisture, float temperature) const;
    const std::vector<BiomeDefinition>& getAllBiomes() const;
};

#endif // BIOMESYSTEM_H