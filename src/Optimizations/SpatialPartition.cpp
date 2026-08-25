// src/Optimizations/SpatialPartition.cpp
#include "Optimizations/SpatialPartition.h"
#include "Entities/Entity.h"
#include <algorithm>
#include <iostream>
#include <memory>
SpatialPartition::SpatialPartition(float cellSize, int gridWidth, int gridHeight)
    : cellSize(cellSize), gridWidth(gridWidth), gridHeight(gridHeight) {}

int SpatialPartition::hash(int x, int y) const {
    // Простая хэш-функция для 2D координат
    // Предполагаем, что x и y неотрицательны и помещаются в gridWidth/Height
    return x + y * gridWidth;
}

void SpatialPartition::addEntity(const std::shared_ptr<Entity>& entity, int x, int y) {
    if (!entity) return;
    int cellX = static_cast<int>(x / cellSize);
    int cellY = static_cast<int>(y / cellSize);

    // Проверка границ
    if (cellX < 0 || cellX >= gridWidth || cellY < 0 || cellY >= gridHeight) {
        // std::cerr << "Предупреждение: Сущность " << entity->getName() << " вне границ SpatialPartition (" << cellX << ", " << cellY << ")" << std::endl;
        return;
    }

    int key = hash(cellX, cellY);
    grid[key].entities.push_back(entity);
}

void SpatialPartition::removeEntity(const std::shared_ptr<Entity>& entity, int x, int y) {
    if (!entity) return;
    int cellX = static_cast<int>(x / cellSize);
    int cellY = static_cast<int>(y / cellSize);

    if (cellX < 0 || cellX >= gridWidth || cellY < 0 || cellY >= gridHeight) {
        return;
    }

    int key = hash(cellX, cellY);
    auto it = grid.find(key);
    if (it != grid.end()) {
        auto& entities = it->second.entities;
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [&entity](const std::shared_ptr<Entity>& e) { return e == entity; }),
            entities.end()
        );
        // Если ячейка стала пустой, можно её удалить (опционально)
        // if (entities.empty()) {
        //     grid.erase(it);
        // }
    }
}

void SpatialPartition::updateEntity(const std::shared_ptr<Entity>& entity, int oldX, int oldY, int newX, int newY) {
    // Упрощенная реализация: удалить из старой и добавить в новую
    removeEntity(entity, oldX, oldY);
    addEntity(entity, newX, newY);
}

std::vector<std::shared_ptr<Entity>> SpatialPartition::getNearbyEntities(int x, int y) const {
    std::vector<std::shared_ptr<Entity>> result;
    int cellX = static_cast<int>(x / cellSize);
    int cellY = static_cast<int>(y / cellSize);

    if (cellX < 0 || cellX >= gridWidth || cellY < 0 || cellY >= gridHeight) {
        return result; // Возвращаем пустой вектор
    }

    // Проверяем текущую и соседние 8 ячеек (3x3 сетка)
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int checkX = cellX + dx;
            int checkY = cellY + dy;
            // Проверка границ сетки
            if (checkX >= 0 && checkX < gridWidth && checkY >= 0 && checkY < gridHeight) {
                int key = hash(checkX, checkY);
                auto it = grid.find(key);
                if (it != grid.end()) {
                    // Добавляем все сущности из найденной ячейки
                    result.insert(result.end(), it->second.entities.begin(), it->second.entities.end());
                }
            }
        }
    }
    return result;
}