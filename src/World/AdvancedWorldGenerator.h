// World/AdvancedWorldGenerator.h
#ifndef ADVANCEDWORLDGENERATOR_H
#define ADVANCEDWORLDGENERATOR_H
#include "Utils/PerlinNoise.h"
#include "World/GameWorld.h"
#include "World/TileBasedLocation.h"
#include "World/TileMap.h"
#include "World/BiomeSystem.h"
#include <future> 
#include <memory>
#include <random>
#include <string>
#include <thread> 
#include <vector> 

#include <iostream>

#include <algorithm>
#include <chrono>
#include <cmath>

class AdvancedWorldGenerator {
public:
    struct GenerationParams {
        // === Основные параметры мира ===
        int worldSize = 512;
        int regionSize = 64;
        float waterLevel = 0.4f;

        // === Параметры высоты (базовый ландшафт) ===
        float baseScale = 0.005f;      // Масштаб базового шума (меньше = крупнее детали)
        int baseOctaves = 5;           // Количество октав для высоты
        float basePersistence = 0.5f;  // Устойчивость октав (меньше = плавнее)

        // === Параметры гор ===
        float mountainHeight = 0.7f;
        float mountainScale = 0.02f;
        int mountainOctaves = 4;
        float mountainPersistence = 0.5f;

        // === Параметры лесов ===
        float forestThreshold = 0.6f;
        float forestScale = 0.05f;
        int forestOctaves = 3;
        float forestPersistence = 0.6f;

        // === Параметры рек ===
        int numRivers = 5;
        float riverCurvature = 0.5f;
    };
    void setParams(const GenerationParams& newParams);
    const GenerationParams& getParams() const;
    // Добавлено: методы для настройки отдельных параметров
    void setWorldSize(int size) { params.worldSize = size; }
    void setRegionSize(int size) { params.regionSize = size; }
    void setWaterLevel(float level) { params.waterLevel = level; }
    void setMountainHeight(float height) { params.mountainHeight = height; }
    void setMountainScale(float scale) { params.mountainScale = scale; }
    void setMountainOctaves(int octaves) { params.mountainOctaves = octaves; }
    void setMountainPersistence(float persistence) { params.mountainPersistence = persistence; }
    void setForestThreshold(float threshold) { params.forestThreshold = threshold; }
    void setForestScale(float scale) { params.forestScale = scale; }
    void setForestOctaves(int octaves) { params.forestOctaves = octaves; }
    void setForestPersistence(float persistence) { params.forestPersistence = persistence; }
    void setNumRivers(int num) { params.numRivers = num; }
    void setRiverCurvature(float curvature) { params.riverCurvature = curvature; }
    // Методы генерации
    std::unique_ptr<GameWorld> generateWorld(const std::string& worldName);
private:
    std::unique_ptr<PerlinNoise> baseNoise;
    std::unique_ptr<PerlinNoise> mountainNoise;
    std::unique_ptr<PerlinNoise> forestNoise;
    GenerationParams params;
    std::mt19937 gen;
    BiomeSystem biomeSystem;

    // Вспомогательные методы
    void generateTerrain(TileBasedLocation* location);
    void generateMountains(TileBasedLocation* location);
    void generateRivers(TileBasedLocation* location);
    void generateForests(TileBasedLocation* location);
    void generateVillage(TileBasedLocation* location, int villageX, int villageY, int villageSize); 
public:
    AdvancedWorldGenerator(unsigned int seed = 0);
    ~AdvancedWorldGenerator() = default;
    // Запрет копирования
    AdvancedWorldGenerator(const AdvancedWorldGenerator&) = delete;
    AdvancedWorldGenerator& operator=(const AdvancedWorldGenerator&) = delete;
};
#endif // ADVANCEDWORLDGENERATOR_H