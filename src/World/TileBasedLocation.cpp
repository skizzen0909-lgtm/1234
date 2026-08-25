// World/TileBasedLocation.cpp
#include "TileBasedLocation.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Optimizations/SpatialPartition.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cmath>

TileBasedLocation::TileBasedLocation(const std::string& name, const std::string& description,
    int width, int height, int offsetX, int offsetY)
    : Location(name, description),
    tileMap(std::make_unique<TileMap>(width, height)),
    spatialPartition(std::make_unique<SpatialPartition>(10.0f, (width / 10) + 2, (height / 10) + 2)) {

    
    heightMap.resize(height, std::vector<float>(width, 0.0f));
    moistureMap.resize(height, std::vector<float>(width, 0.0f));
    temperatureMap.resize(height, std::vector<float>(width, 0.0f));
}

TileMap* TileBasedLocation::getTileMap() const {
    return tileMap.get();
}

SpatialPartition* TileBasedLocation::getSpatialPartition() const {
    return spatialPartition.get();
}

bool TileBasedLocation::addEntity(std::shared_ptr<Entity> entity) {
    if (!entity || !Location::addEntity(entity)) {
        return false; // Базовый класс уже выдал ошибку
    }

    auto pos = entity->getPosition();
    // Добавлена явная проверка на (-1, -1) перед добавлением в tileMap/spatialPartition
    if (pos.first != -1 && pos.second != -1) {
        if (tileMap) {
            tileMap->addEntityToTile(entity, pos.first, pos.second);
        }
        if (spatialPartition) {
            spatialPartition->addEntity(entity, pos.first, pos.second);
        }
    }
    else {
        // Логируем или обрабатываем ситуацию, когда сущность добавляется с недопустимой позицией
        // Это может быть нормально на этапе создания, но требует спавна
        std::cerr << "TileBasedLocation::addEntity: Warning - Adding entity '" << entity->getName()
            << "' with invalid position (-1, -1). It will not be added to tile map or spatial partition yet." << std::endl;
        // Возвращаем true, так как добавление в список Location прошло успешно
        // Но сущность не будет видна на карте/в поиске до получения допустимой позиции
    }
    return true;
}

void TileBasedLocation::setTileMap(std::unique_ptr<TileMap> newMap) {
    tileMap = std::move(newMap);
}
void TileBasedLocation::removeEntity(std::shared_ptr<Entity> entity) {
    if (!entity) return;

    auto pos = entity->getPosition();
    // Добавлена явная проверка на (-1, -1) перед удалением из tileMap/spatialPartition
    if (pos.first != -1 && pos.second != -1) {
        if (tileMap) {
            tileMap->removeEntityFromTile(entity, pos.first, pos.second);
        }
        if (spatialPartition) {
            spatialPartition->removeEntity(entity, pos.first, pos.second);
        }
    }
    else {
        // Логируем, если пытаемся удалить сущность с недопустимой позицией
        // Это может быть нормально, если она не была добавлена или уже удалена
        // std::cerr << "TileBasedLocation::removeEntity: Warning - Removing entity '" << (entity ? entity->getName() : "nullptr")
        //           << "' with invalid position (-1, -1) or already removed." << std::endl;
    }
    // Всегда вызываем базовую реализацию для удаления из списка Location
    Location::removeEntity(entity);
}
void TileBasedLocation::setHeightMap(std::vector<std::vector<float>>&& newMap) {
    heightMap = std::move(newMap);
}

void TileBasedLocation::setMoistureMap(std::vector<std::vector<float>>&& newMap) {
    moistureMap = std::move(newMap);
}

void TileBasedLocation::setTemperatureMap(std::vector<std::vector<float>>&& newMap) {
    temperatureMap = std::move(newMap);
}
std::vector<std::shared_ptr<Entity>> TileBasedLocation::getEntitiesInRange(int centerX, int centerY, int range) const {
    std::vector<std::shared_ptr<Entity>> result;
    if (!tileMap) return result;
    for (int dy = -range; dy <= range; ++dy) {
        for (int dx = -range; dx <= range; ++dx) {
            int x = centerX + dx;
            int y = centerY + dy;
            if (tileMap->isValidPosition(x, y)) {
                const auto& entitiesOnTile = tileMap->getEntities(x, y);
                result.insert(result.end(), entitiesOnTile.begin(), entitiesOnTile.end());
            }
        }
    }
    return result;
}

std::pair<int, int> TileBasedLocation::findSpawnPoint() const {
    // Ищем тайл типа SPAWN
    if (!tileMap) return { -1, -1 };

    int width = tileMap->getWidth();
    int height = tileMap->getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tileMap->getTile(x, y) == TileType::SPAWN) {
                // Проверяем, что на нем нет объектов и декораций (дополнительная защита)
                const TileLayer& layer = tileMap->getLayer(x, y);
                if (layer.objectType == TileType::VOID && layer.decorationLayers.empty()) {
                    return { x, y };
                }
            }
        }
    }

    // Если SPAWN не найден — возвращаем центр как fallback
    int centerX = width / 2;
    int centerY = height / 2;
    if (tileMap->isWalkable(centerX, centerY)) {
        return { centerX, centerY };
    }

    // Если и центр не проходим — ищем любое проходимое место
    return tileMap->findEmptyWalkableTile();
}

std::shared_ptr<Player> TileBasedLocation::findPlayer() const {
    for (const auto& entity : entities) {
        if (auto player = std::dynamic_pointer_cast<Player>(entity)) {
            return player;
        }
    }
    return nullptr;
}

std::pair<int, int> TileBasedLocation::findNearbyWalkableTile(int centerX, int centerY, int radius) const {
    if (!tileMap) return { -1, -1 };

    std::vector<std::pair<int, int>> candidates;
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int x = centerX + dx;
            int y = centerY + dy;
            if (tileMap->isValidPosition(x, y) && tileMap->isWalkable(x, y)) {
                candidates.emplace_back(x, y);
            }
        }
    }

    if (candidates.empty()) return { -1, -1 };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(candidates.size()) - 1);
    return candidates[dist(gen)];
}