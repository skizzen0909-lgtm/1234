// World/Location.cpp
#include "Location.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"

Location::Location(const std::string& locName, const std::string& locDesc)
    : name(locName), description(locDesc) {
}

std::shared_ptr<Player> Location::findPlayer() const {
    for (const auto& entity : entities) {
        if (auto player = std::dynamic_pointer_cast<Player>(entity)) {
            return player;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> Location::getEntitiesInRange(int centerX, int centerY, int range) const {
    // В базовой реализации Location не имеет координат, поэтому возвращаем все сущности.
    // Это упрощение. В TileBasedLocation будет более точная реализация.
    return entities;
}

bool Location::addEntity(std::shared_ptr<Entity> entity) {
    if (!entity) {
        return false; // Не удалось добавить
    }
    entities.push_back(entity);
    return true; // Успешно добавлено
}

void Location::removeEntity(std::shared_ptr<Entity> entity) {
    if (entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }
}