// World/TileBasedLocation.h
#ifndef TILEBASEDLOCATION_H
#define TILEBASEDLOCATION_H

#include <memory>
#include <string>
#include <vector>
#include "Location.h"
#include "TileMap.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Optimizations/SpatialPartition.h"

class TileBasedLocation : public Location {
private:
    std::unique_ptr<TileMap> tileMap;
    std::unique_ptr<SpatialPartition> spatialPartition;
    // Карты параметров ландшафта
    std::vector<std::vector<float>> heightMap;
    std::vector<std::vector<float>> moistureMap;
    std::vector<std::vector<float>> temperatureMap;

public:
    TileBasedLocation(const std::string& name, const std::string& description,
        int width, int height, int offsetX = 0, int offsetY = 0);
    ~TileBasedLocation() override = default;

    // Геттеры
    TileMap* getTileMap() const;
    SpatialPartition* getSpatialPartition() const;

    // Управление картами
    void setHeightMap(std::vector<std::vector<float>>&& newMap);
    void setMoistureMap(std::vector<std::vector<float>>&& newMap);
    void setTemperatureMap(std::vector<std::vector<float>>&& newMap);

    // Константные геттеры для карт
    const std::vector<std::vector<float>>& getHeightMap() const { return heightMap; }
    const std::vector<std::vector<float>>& getMoistureMap() const { return moistureMap; }
    const std::vector<std::vector<float>>& getTemperatureMap() const { return temperatureMap; }

    // Управление сущностями и локацией
    std::pair<int, int> findSpawnPoint() const;
    std::shared_ptr<Player> findPlayer() const override;
    std::vector<std::shared_ptr<Entity>> getEntitiesInRange(int centerX, int centerY, int range) const override;
    std::pair<int, int> findNearbyWalkableTile(int centerX, int centerY, int radius) const;
    bool addEntity(std::shared_ptr<Entity> entity) override;
    void removeEntity(std::shared_ptr<Entity> entity) override;
    void setTileMap(std::unique_ptr<TileMap> newMap);

    // Запрет копирования
    TileBasedLocation(const TileBasedLocation&) = delete;
    TileBasedLocation& operator=(const TileBasedLocation&) = delete;
};

#endif // TILEBASEDLOCATION_H