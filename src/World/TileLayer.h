// World/TileLayer.h
#ifndef TILELAYER_H
#define TILELAYER_H
#include "Types/Types.h"
#include <vector>
#include <memory>
#include <algorithm>

class Entity;

enum class LayerType {
    BASE,      // Основание: GROUND, WATER, MOUNTAIN, ROAD, BUILDING, VOID
    OBJECT,    // Объекты: TREE, HOUSE, ROAD_SEGMENT - ТОЛЬКО ОДИН НА ТАЙЛ!
    DECORATION // Декор: КАМНИ, ЦВЕТЫ, ТРАВА - НЕ ВЛИЯЕТ НА ПРОХОДИМОСТЬ
};

struct TileLayer {
    TileType baseType = TileType::GROUND;
    int groundVariant = 0; 
    TileType objectType = TileType::VOID;
    std::vector<TileType> decorationLayers;
    std::vector<std::shared_ptr<Entity>> entities;

    TileLayer() = default;

    bool isWalkable() const {
        // Основание
        switch (baseType) {
        case TileType::WATER:
        case TileType::VOID:
            return false;
        case TileType::SPAWN:
            // SPAWN всегда проходим
            return true;
        case TileType::MOUNTAIN:
        case TileType::BUILDING:
            // Эти типы как основание — НЕ проходимы
            return false;
        default:
            break; // GROUND, ROAD_SEGMENT и т.п. — проходимы
        }

        // Объекты — они могут блокировать проход!
        switch (objectType) {
        case TileType::MOUNTAIN:
        case TileType::BUILDING:
            return false; // Эти объекты — непроходимы!
        default:
            break; // TREE, HOUSE, ROAD_SEGMENT — проходимы (они на GROUND)
        }

        // Декорации — всегда проходимы
        return true;
    }

    // Добавить объект (проверка на совместимость и уникальность)
    bool addObject(TileType objType) {
        // Нельзя добавлять объекты на воду
        if (baseType == TileType::WATER) {
            return false;
        }
        // Только один объект на тайл
        if (objectType != TileType::VOID) {
            return false;
        }
        // Проверка типов объектов
        switch (objType) {
        case TileType::TREE:
        case TileType::HOUSE:
        case TileType::ROAD_SEGMENT:
        case TileType::RUIN:
            // Эти объекты могут быть только на GROUND
            if (baseType != TileType::GROUND) {
                return false;
            }
            break;
        case TileType::MOUNTAIN:
        case TileType::BUILDING:
            // Эти объекты ЗАМЕНЯЮТ собой GROUND, но не могут быть на WATER
            // Они становятся основным объектом
            break;
        default:
            // Любые другие типы не являются объектами
            return false;
        }
        objectType = objType;
        return true;
    }

    // Удалить объект
    bool removeObject(TileType objType) {
        if (objectType == objType) {
            objectType = TileType::VOID;
            return true;
        }
        return false;
    }

    void addDecoration(TileType decType) {
        if (decorationLayers.size() >= 3) return;
        // Декор можно добавлять ТОЛЬКО на траву (GROUND)
        if (baseType != TileType::GROUND) return;
        decorationLayers.push_back(decType);
    }

    // Получить все типы для рендера (порядок: база -> объект -> декор)
    std::vector<TileType> getAllRenderTypes() const {
        std::vector<TileType> result;
        result.push_back(baseType);
        if (objectType != TileType::VOID) {
            result.push_back(objectType);
        }
        result.insert(result.end(), decorationLayers.begin(), decorationLayers.end());
        return result;
    }

    // Получить объект (если есть)
    TileType getObjectType() const { return objectType; }
};
#endif