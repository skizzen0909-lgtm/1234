// src/World/BiomeSystem.cpp
#include "BiomeSystem.h"
#include <algorithm>
#include <iostream>

BiomeSystem::BiomeSystem() {
    initializeDefaultBiomes();
}

void BiomeSystem::initializeDefaultBiomes() {
    biomes.clear();

    // 1. Океан
    biomes.push_back({
        "Ocean", TileType::WATER,
        {}, {},
        0.0f, 0.35f,
        0.0f, 1.0f,
        -1.0f, 1.0f
        });

    // 2. Пляж
    biomes.push_back({
        "Beach", TileType::SAND,
        {TileType::STONE}, {},
        0.35f, 0.4f,
        0.0f, 1.0f,
        -1.0f, 1.0f
        });

    // 3. Равнины
    biomes.push_back({
        "Plains", TileType::GRASS,
        {TileType::GRASS_PATCH, TileType::FLOWER}, {TileType::STONE},
        0.4f, 0.6f,
        0.3f, 0.8f,
        -0.5f, 0.8f
        });

    // 4. Лес
    biomes.push_back({
        "Forest", TileType::GRASS,
        {TileType::TREE}, {TileType::FLOWER, TileType::RUIN},
        0.4f, 0.65f,
        0.6f, 1.0f,
        -0.3f, 0.7f
        });

    // 5. Тайга (хвойный лес)
    biomes.push_back({
        "Taiga", TileType::SNOW,
        {TileType::TREE}, {TileType::STONE},
        0.4f, 0.65f,
        0.4f, 0.9f,
        -1.0f, -0.2f
        });

    // 6. Тундра
    biomes.push_back({
        "Tundra", TileType::SNOW,
        {TileType::STONE}, {},
        0.4f, 0.6f,
        0.0f, 0.5f,
        -1.0f, -0.3f
        });

    // 7. Пустыня
    biomes.push_back({
        "Desert", TileType::SAND,
        {TileType::STONE}, {TileType::RUIN},
        0.4f, 0.65f,
        0.0f, 0.3f,
        0.2f, 1.0f
        });

    // 8. Горы
    biomes.push_back({
        "Mountains", TileType::ROCK,
        {TileType::MOUNTAIN}, {TileType::RUIN},
        0.65f, 1.0f,
        0.0f, 1.0f,
        -1.0f, 1.0f
        });
}

BiomeDefinition BiomeSystem::determineBiome(float height, float moisture, float temperature) const {
    for (const auto& biome : biomes) {
        if (height >= biome.minHeight && height <= biome.maxHeight &&
            moisture >= biome.minMoisture && moisture <= biome.maxMoisture &&
            temperature >= biome.minTemperature && temperature <= biome.maxTemperature) {
            return biome;
        }
    }
    // fallback
    return biomes[0]; // Ocean
}

BiomeDefinition BiomeSystem::getBiomeAt(float height, float moisture, float temperature) const {
    return determineBiome(height, moisture, temperature);
}

const std::vector<BiomeDefinition>& BiomeSystem::getAllBiomes() const {
    return biomes;
}