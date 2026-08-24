// World/TileMap.cpp
#include "World/TileMap.h"
#include "Entities/Entity.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>

TileMap::TileMap(int width, int height) : width(width), height(height) {
    if (width <= 0 || height <= 0) {
        std::cerr << "TileMap constructor: Invalid dimensions (" << width << ", " << height << ")" << std::endl;
        width = std::max(1, width);
        height = std::max(1, height);
    }
    tiles.assign(height, std::vector<TileType>(width, TileType::GROUND));
    layers.assign(height, std::vector<TileLayer>(width));
    std::cout << "TileMap created with dimensions: " << width << "x" << height << std::endl;
}

int TileMap::getWidth() const { return width; }
int TileMap::getHeight() const { return height; }

bool TileMap::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool TileMap::isWalkable(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return layers[y][x].isWalkable();
}

void TileMap::setTile(int x, int y, TileType type) {
    if (isValidPosition(x, y)) {
        layers[y][x].baseType = type;
    }
}

TileType TileMap::getTile(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return TileType::VOID;
    }
    if (tiles.empty() || y >= static_cast<int>(tiles.size()) || x >= static_cast<int>(tiles[y].size())) {
        std::cerr << "FATAL: getTile() called with out-of-bounds coordinates: ("
            << x << "," << y << ") but tiles size: "
            << tiles.size() << "x" << (tiles.empty() ? 0 : tiles[0].size()) << std::endl;
        return TileType::VOID;
    }
    return tiles[y][x];
}

bool TileMap::addObjectToTile(int x, int y, TileType objType) {
    if (!isValidPosition(x, y)) return false;
    TileLayer& layer = layers[y][x];
    TileType baseType = layer.baseType;

    // Нельзя ставить объекты на воду или пустоту
    if (baseType == TileType::WATER || baseType == TileType::VOID) {
        return false;
    }

    // Нельзя ставить объект на SPAWN
    if (baseType == TileType::SPAWN) {
        return false;
    }

    // Уже есть объект?
    if (layer.objectType != TileType::VOID) {
        return false;
    }

    // Проверка совместимости объекта с базовым типом
    switch (objType) {
    case TileType::TREE:
    case TileType::HOUSE:
    case TileType::ROAD_SEGMENT:
    case TileType::RUIN:
        if (baseType != TileType::GROUND) return false;
        break;
    case TileType::MOUNTAIN:
    case TileType::BUILDING:
        // Эти типы — базовые, не объекты!
        return false;
    default:
        return false; // Неизвестный объект
    }

    layer.objectType = objType;
    return true;
}

void TileMap::addDecorationToTile(int x, int y, TileType decorationType) {
    if (!isValidPosition(x, y)) {
        std::cerr << "TileMap::addDecorationToTile: Invalid coordinates (" << x << ", " << y << ")" << std::endl;
        return;
    }
    TileLayer& layer = layers[y][x];
    TileType baseType = layer.baseType;

    // Запрещено на SPAWN, WATER, VOID
    if (baseType == TileType::SPAWN || baseType == TileType::WATER || baseType == TileType::VOID) {
        return;
    }

    // Декорации — только на GROUND
    if (baseType == TileType::GROUND) {
        layer.addDecoration(decorationType);
    }
}

const std::vector<std::shared_ptr<Entity>>& TileMap::getEntities(int x, int y) const {
    static const std::vector<std::shared_ptr<Entity>> emptyEntities;
    if (!isValidPosition(x, y)) {
        return emptyEntities;
    }
    return layers[y][x].entities;
}

bool TileMap::addEntityToTile(std::shared_ptr<Entity> entity, int x, int y) {
    if (!entity || !isValidPosition(x, y) || !isWalkable(x, y)) {
        return false;
    }
    TileLayer& layer = layers[y][x];
    if (std::find(layer.entities.begin(), layer.entities.end(), entity) == layer.entities.end()) {
        layer.entities.push_back(entity);
        return true;
    }
    return false;
}

bool TileMap::removeEntityFromTile(std::shared_ptr<Entity> entity, int x, int y) {
    if (!entity || !isValidPosition(x, y)) {
        return false;
    }
    TileLayer& layer = layers[y][x];
    auto it = std::find(layer.entities.begin(), layer.entities.end(), entity);
    if (it != layer.entities.end()) {
        layer.entities.erase(it);
        return true;
    }
    return false;
}

std::pair<int, int> TileMap::findEmptyWalkableTile() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, width - 1);
    std::uniform_int_distribution<> yDist(0, height - 1);
    const int maxAttempts = width * height;

    for (int i = 0; i < maxAttempts; ++i) {
        int x = xDist(gen);
        int y = yDist(gen);
        if (isValidPosition(x, y) && isWalkable(x, y) && layers[y][x].entities.empty()) {
            return { x, y };
        }
    }

    // Линейный fallback
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (isValidPosition(x, y) && isWalkable(x, y) && layers[y][x].entities.empty()) {
                return { x, y };
            }
        }
    }

    std::cerr << "TileMap::findEmptyWalkableTile: FAILED. No walkable, empty tile found!" << std::endl;
    return { -1, -1 };
}

const TileLayer& TileMap::getLayer(int x, int y) const {
    static const TileLayer invalidLayer;
    if (!isValidPosition(x, y)) {
        std::cerr << "TileMap::getLayer (const): Invalid position (" << x << ", " << y << ")." << std::endl;
        return invalidLayer;
    }
    return layers[y][x];
}

TileLayer& TileMap::getLayer(int x, int y) {
    static TileLayer invalidLayer;
    if (!isValidPosition(x, y)) {
        std::cerr << "TileMap::getLayer: Invalid position (" << x << ", " << y << ")." << std::endl;
        return invalidLayer;
    }
    return layers[y][x];
}