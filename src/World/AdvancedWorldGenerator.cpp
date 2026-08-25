// World/AdvancedWorldGenerator.cpp
#include "World/AdvancedWorldGenerator.h"
#include "World/TileBasedLocation.h"
#include "World/TileMap.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/VillagerNPC.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <memory>
#include <thread>
#include <future>
#include <chrono>

AdvancedWorldGenerator::AdvancedWorldGenerator(unsigned int seed)
    : baseNoise(std::make_unique<PerlinNoise>(seed)),
    mountainNoise(std::make_unique<PerlinNoise>(seed + 1000)),
    forestNoise(std::make_unique<PerlinNoise>(seed + 2000)),
    gen(seed),
    biomeSystem()
{
    // Конструктор ничего не делает, кроме инициализации
}

void AdvancedWorldGenerator::setParams(const GenerationParams& newParams) {
    params = newParams;
}

const AdvancedWorldGenerator::GenerationParams& AdvancedWorldGenerator::getParams() const {
    return params;
}

// World/AdvancedWorldGenerator.cpp
std::unique_ptr<GameWorld> AdvancedWorldGenerator::generateWorld(const std::string& worldName) {
    auto world = std::make_unique<GameWorld>(worldName);
    // Создаем континент с параметрами
    auto continent = std::make_unique<TileBasedLocation>(
        "Континент",
        "Обширный континент с разнообразными ландшафтами.",
        params.worldSize,
        params.worldSize
    );
    // --- МНОГОПОТОЧНАЯ ГЕНЕРАЦИЯ ---
    std::cout << "Начало многопоточной генерации..." << std::endl;
    // 1. Генерация базовой карты (земля/вода)
    auto terrainFuture = std::async(std::launch::async, &AdvancedWorldGenerator::generateTerrain, this, continent.get());
    // 2. Генерация гор
    auto mountainFuture = std::async(std::launch::async, &AdvancedWorldGenerator::generateMountains, this, continent.get());
    // 3. Генерация рек
    auto riverFuture = std::async(std::launch::async, &AdvancedWorldGenerator::generateRivers, this, continent.get());
    // 4. Генерация лесов
    auto forestFuture = std::async(std::launch::async, &AdvancedWorldGenerator::generateForests, this, continent.get());
    // --- ОЖИДАНИЕ ЗАВЕРШЕНИЯ ВСЕХ ПОТОКОВ ---
    terrainFuture.get();
    mountainFuture.get();
    riverFuture.get();
    forestFuture.get();
    std::cout << "Многопоточная генерация завершена." << std::endl;
    // --- Генерация деревни (после основного ландшафта) ---
    int villageSize = 50;
    int villageX = params.worldSize / 2;
    int villageY = params.worldSize / 2;
    generateVillage(continent.get(), villageX, villageY, villageSize);

    world->addLocation(std::move(continent));
    world->setCurrentLocation(0);
    std::cout << "Мир '" << worldName << "' сгенерирован." << std::endl;
    return world;
}

void AdvancedWorldGenerator::generateVillage(TileBasedLocation* location, int villageX, int villageY, int villageSize) {
    if (!location) {
        std::cerr << "Ошибка: generateVillage требует TileBasedLocation." << std::endl;
        return;
    }
    TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "Ошибка: TileMap не найдена в локации." << std::endl;
        return;
    }

    // Убедимся, что центр деревни — GROUND
    int centerX = villageX;
    int centerY = villageY;

    // 1. Очистка площади деревни: устанавливаем GROUND
    for (int dy = -villageSize / 2; dy <= villageSize / 2; ++dy) {
        for (int dx = -villageSize / 2; dx <= villageSize / 2; ++dx) {
            int nx = centerX + dx;
            int ny = centerY + dy;
            if (tileMap->isValidPosition(nx, ny)) {
                tileMap->setTile(nx, ny, TileType::GROUND);
            }
        }
    }

    // 2. Спавн точки игрока
    int spawnX = centerX;
    int spawnY = centerY - 3;
    if (tileMap->isValidPosition(spawnX, spawnY)) {
        tileMap->setTile(spawnX, spawnY, TileType::SPAWN);
        TileLayer& spawnLayer = tileMap->getLayer(spawnX, spawnY);
        spawnLayer.objectType = TileType::VOID;
        spawnLayer.decorationLayers.clear();
    }

    // 3. Генерация дорог (простой крест)
    for (int i = -10; i <= 10; ++i) {
        if (tileMap->isValidPosition(centerX + i, centerY)) {
            tileMap->addObjectToTile(centerX + i, centerY, TileType::ROAD_SEGMENT);
        }
        if (tileMap->isValidPosition(centerX, centerY + i)) {
            tileMap->addObjectToTile(centerX, centerY + i, TileType::ROAD_SEGMENT);
        }
    }

    // 4. Дома по краям
    for (int dx : {-8, 8}) {
        for (int dy : {-8, 8}) {
            int hx = centerX + dx;
            int hy = centerY + dy;
            if (tileMap->isValidPosition(hx, hy)) {
                tileMap->addObjectToTile(hx, hy, TileType::HOUSE);
            }
        }
    }
    // 5. Добавляем NPC
    if (location && location->getTileMap()) {
        // Создаём торговца рядом с центром
        auto trader = std::make_shared<VillagerNPC>("Торговец", "торговец", 50);
        int traderX = villageX - 2;
        int traderY = villageY;
        if (location->getTileMap()->isWalkable(traderX, traderY)) {
            trader->setPosition(traderX, traderY);
            location->addEntity(trader);
            std::cout << "Создан NPC 'Торговец' в (" << traderX << ", " << traderY << ")" << std::endl;
        }

        // Создаём фермера
        auto farmer = std::make_shared<VillagerNPC>("Фермер Иван", "фермер", 50);
        int farmerX = villageX + 3;
        int farmerY = villageY + 2;
        if (location->getTileMap()->isWalkable(farmerX, farmerY)) {
            farmer->setPosition(farmerX, farmerY);
            location->addEntity(farmer);
            std::cout << "Создан NPC 'Фермер Иван' в (" << farmerX << ", " << farmerY << ")" << std::endl;
        }
    }
    std::cout << "Деревня сгенерирована в (" << villageX << ", " << villageY << ")" << std::endl;
}

void AdvancedWorldGenerator::generateTerrain(TileBasedLocation* location) {
    if (!location) {
        std::cerr << "Error: generateTerrain requires TileBasedLocation." << std::endl;
        return;
    }
    TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "Error: TileMap not found in location." << std::endl;
        return;
    }
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();

    // === 1. Создаём локальные карты параметров ===
    std::vector<std::vector<float>> heightMap(height, std::vector<float>(width));
    std::vector<std::vector<float>> moistureMap(height, std::vector<float>(width));
    std::vector<std::vector<float>> temperatureMap(height, std::vector<float>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float nx = static_cast<float>(x) * params.baseScale;
            float ny = static_cast<float>(y) * params.baseScale;
            heightMap[y][x] = baseNoise->fractalNoise(nx, ny, params.baseOctaves, params.basePersistence);
            moistureMap[y][x] = baseNoise->fractalNoise(nx + 100.0f, ny + 100.0f, 3, 0.6f);
            temperatureMap[y][x] = (1.0f - static_cast<float>(y) / height) +
                (baseNoise->fractalNoise(nx + 200.0f, ny + 200.0f, 2, 0.5f) * 0.2f);
        }
    }

    // === 2. Применяем биомы и устанавливаем groundVariant ===
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float h = heightMap[y][x];
            float m = moistureMap[y][x];
            float t = temperatureMap[y][x];
            auto biome = biomeSystem.getBiomeAt(h, m, t);

            // Устанавливаем базовый тип тайла
            tileMap->setTile(x, y, biome.baseTile);

            // === ИСПРАВЛЕНИЕ: назначаем случайный вариант для GROUND ===
            if (biome.baseTile == TileType::GROUND) {
                int variant = gen() % 4; // 0, 1, 2, 3
                tileMap->getLayer(x, y).groundVariant = variant;
            }

            // === 3. Добавляем объекты на тайл ===
            if (!biome.commonObjects.empty()) {
                std::uniform_int_distribution<> dist(0, static_cast<int>(biome.commonObjects.size()) - 1);
                int index = dist(gen);
                tileMap->addObjectToTile(x, y, biome.commonObjects[index]);
            }
            if (!biome.rareObjects.empty() && (gen() % 100) < 5) {
                std::uniform_int_distribution<> dist(0, static_cast<int>(biome.rareObjects.size()) - 1);
                int index = dist(gen);
                tileMap->addObjectToTile(x, y, biome.rareObjects[index]);
            }
        }
    }
    std::cout << "Базовая карта (биомы) сгенерирована." << std::endl;
}
void AdvancedWorldGenerator::generateMountains(TileBasedLocation* location) {
    if (!location) {
        std::cerr << "Ошибка: generateMountains требует TileBasedLocation." << std::endl;
        return;
    }
    TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "Ошибка: TileMap не найдена в локации." << std::endl;
        return;
    }
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();
    const auto& heightMap = location->getHeightMap();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float heightVal = heightMap[y][x];
            TileType currentBase = tileMap->getTile(x, y);

            // Горы только на суше: GROUND или DESERT
            if (heightVal >= params.mountainHeight &&
                (currentBase == TileType::GROUND || currentBase == TileType::DESERT)) {
                tileMap->setTile(x, y, TileType::MOUNTAIN);
            }
        }
    }
    std::cout << "Горы сгенерированы." << std::endl;
}

void AdvancedWorldGenerator::generateRivers(TileBasedLocation* location) {
    if (!location) {
        std::cerr << "Ошибка: generateRivers требует TileBasedLocation." << std::endl;
        return;
    }
    TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "Ошибка: TileMap не найдена в локации." << std::endl;
        return;
    }
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();
    std::uniform_int_distribution<> xDist(0, width - 1);
    std::uniform_int_distribution<> yDist(0, height - 1);

    // Получаем сохраненные карты из TileBasedLocation
    const auto& heightMap = location->getHeightMap();
    const auto& moistureMap = location->getMoistureMap();
    const auto& temperatureMap = location->getTemperatureMap();

    for (int r = 0; r < params.numRivers; ++r) {
        // 1. Определяем начальную точку
        int startX = xDist(gen);
        int startY = yDist(gen);
        int attempts = 0;
        const int maxAttempts = 1000;

        // Ищем точку на суше или берегу
        while (attempts < maxAttempts) {
            // Используем реальные значения из карт для точки (startX, startY)
            float heightVal = heightMap[startY][startX];
            float moistureVal = moistureMap[startY][startX];
            float temperatureVal = temperatureMap[startY][startX];

            BiomeDefinition biome = biomeSystem.getBiomeAt(heightVal, moistureVal, temperatureVal);
            if (biome.name != "Ocean" && biome.name != "River") {
                break;
            }
            startX = xDist(gen);
            startY = yDist(gen);
            attempts++;
        }

        if (attempts >= maxAttempts) {
            std::cout << "Не удалось найти стартовую точку для реки " << r << std::endl;
            continue;
        }

        int currentX = startX;
        int currentY = startY;
        std::uniform_real_distribution<> angleDist(-params.riverCurvature, params.riverCurvature);
        float currentAngle = static_cast<float>(gen() % 360) * 3.14159f / 180.0f;
        const int maxRiverLength = std::max(width, height) / 2;

        for (int step = 0; step < maxRiverLength; ++step) {
            if (!tileMap->isValidPosition(currentX, currentY)) {
                break;
            }

            // используем значения из карт для текущей позиции
            BiomeDefinition riverBiome = biomeSystem.getBiomeAt(
                heightMap[currentY][currentX],
                moistureMap[currentY][currentX],
                temperatureMap[currentY][currentX]
            );
            if (riverBiome.name != "River") {
                tileMap->setTile(currentX, currentY, TileType::WATER);
            }

            // Вычисляем следующую позицию
            currentAngle += static_cast<float>(angleDist(gen));
            int nextX = static_cast<int>(currentX + std::round(std::cos(currentAngle)));
            int nextY = static_cast<int>(currentY + std::round(std::sin(currentAngle)));

            // Проверяем, что следующая позиция валидна
            if (!tileMap->isValidPosition(nextX, nextY)) {
                break;
            }

            currentX = nextX;
            currentY = nextY;
        }
    }
    std::cout << "Реки сгенерированы." << std::endl;
}

void AdvancedWorldGenerator::generateForests(TileBasedLocation* location) {
    if (!location) {
        std::cerr << "Ошибка: generateForests требует TileBasedLocation." << std::endl;
        return;
    }
    TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "Ошибка: TileMap не найдена в локации." << std::endl;
        return;
    }
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();

    // Используем карты высоты и влажности из локации
    const auto& heightMap = location->getHeightMap();
    const auto& moistureMap = location->getMoistureMap();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TileType base = tileMap->getTile(x, y);
            // Деревья только на GROUND и не на воде/горах/зданиях
            if (base != TileType::GROUND) continue;

            // Используем реальные карты для определения леса
            float heightVal = heightMap[y][x];
            float moistureVal = moistureMap[y][x];

            // Леса растут на умеренной высоте и высокой влажности
            if (heightVal > params.waterLevel && heightVal < params.mountainHeight &&
                moistureVal > params.forestThreshold) {
                tileMap->addObjectToTile(x, y, TileType::TREE);
            }
        }
    }
    std::cout << "Леса сгенерированы." << std::endl;
}