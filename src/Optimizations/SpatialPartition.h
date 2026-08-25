// src/Optimizations/SpatialPartition.h
#ifndef SPATIALPARTITION_H
#define SPATIALPARTITION_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>

// Forward declaration
class Entity;

// Простая реализация Spatial Hashing Grid
class SpatialPartition {
private:
    struct GridCell {
        std::vector<std::shared_ptr<Entity>> entities;
    };

    float cellSize;
    int gridWidth, gridHeight;
    // Используем хэш-карту для хранения непустых ячеек
    std::unordered_map<int, GridCell> grid;

    // Хэш-функция для координат ячейки (x, y)
    int hash(int x, int y) const;

public:
    SpatialPartition(float cellSize, int gridWidth, int gridHeight);
    ~SpatialPartition() = default;

    // Добавление/удаление сущностей
    void addEntity(const std::shared_ptr<Entity>& entity, int x, int y);
    void removeEntity(const std::shared_ptr<Entity>& entity, int x, int y);
    void updateEntity(const std::shared_ptr<Entity>& entity, int oldX, int oldY, int newX, int newY);

    // Поиск соседей
    std::vector<std::shared_ptr<Entity>> getNearbyEntities(int x, int y) const;

    // Запрет копирования
    SpatialPartition(const SpatialPartition&) = delete;
    SpatialPartition& operator=(const SpatialPartition&) = delete;
};

#endif // SPATIALPARTITION_H