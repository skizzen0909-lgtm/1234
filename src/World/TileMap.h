// World/TileMap.h
#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include "Types/Types.h"
#include "World/TileLayer.h"
#include <vector>
#include <memory>
#include <utility>

class Entity;

class TileMap {
private:
    int width, height;
    std::vector<std::vector<TileLayer>> layers;
    std::vector<std::vector<TileType>> tiles; // Основные тайлы

public:
    TileMap(int width, int height);
    ~TileMap() = default;

    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(int x, int y) const;

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;

    // Добавление объектов и декораций
    bool addObjectToTile(int x, int y, TileType objectType);
    void addDecorationToTile(int x, int y, TileType decorationType);

    const TileLayer& getLayer(int x, int y) const;
    TileLayer& getLayer(int x, int y);

    bool isWalkable(int x, int y) const;

    // Методы для работы с сущностями на тайле
    const std::vector<std::shared_ptr<Entity>>& getEntities(int x, int y) const;
    bool addEntityToTile(std::shared_ptr<Entity> entity, int x, int y);
    bool removeEntityFromTile(std::shared_ptr<Entity> entity, int x, int y);

    // Поиск пустого проходимого тайла
    std::pair<int, int> findEmptyWalkableTile() const;

    // Запрет копирования
    TileMap(const TileMap&) = delete;
    TileMap& operator=(const TileMap&) = delete;
};

#endif // TILEMAP_H