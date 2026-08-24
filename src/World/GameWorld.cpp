// World/GameWorld.cpp
#include "GameWorld.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "World/Location.h"
#include <algorithm>

GameWorld::GameWorld(const std::string& worldName)
    : worldName(worldName), currentLocationIndex(0) {
}

bool GameWorld::addEntity(std::shared_ptr<Entity> entity) {
    if (!entity) {
        return false;
    }
    entities.push_back(entity);
    return true;
}

bool GameWorld::removeEntity(const std::shared_ptr<Entity>& entity) {
    if (!entity) {
        return false;
    }
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        entities.erase(it);
        return true;
    }
    return false;
}

const std::vector<std::shared_ptr<Entity>>& GameWorld::getEntities() const {
    return entities;
}

void GameWorld::addLocation(std::unique_ptr<Location> location) {
    if (location) {
        locations.push_back(std::move(location));
    }
}

Location* GameWorld::getCurrentLocation() const {
    if (currentLocationIndex >= 0 && currentLocationIndex < static_cast<int>(locations.size())) {
        return locations[currentLocationIndex].get();
    }
    return nullptr;
}

bool GameWorld::changeLocation(int locationIndex) {
    if (locationIndex >= 0 && locationIndex < static_cast<int>(locations.size())) {
        currentLocationIndex = locationIndex;
        return true; // Успешно
    }
    return false; // Неудача
}

const std::vector<std::unique_ptr<Location>>& GameWorld::getLocations() const {
    return locations;
}

const std::string& GameWorld::getWorldName() const {
    return worldName;
}

void GameWorld::setWorldName(const std::string& name) {
    worldName = name;
}

int GameWorld::getCurrentLocationIndex() const {
    return currentLocationIndex;
}

void GameWorld::setCurrentLocation(int index) {
    if (index >= 0 && index < locations.size()) {
        currentLocationIndex = index;
    }
}

void GameWorld::update(float deltaTime) {
    // Обновляем все сущности
    for (const auto& entity : entities) {
        if (entity) {
            entity->update(deltaTime);
        }
    }

    // Обновляем текущую локацию
    if (auto* currentLoc = getCurrentLocation()) {
        currentLoc->update(deltaTime);
    }
}

std::shared_ptr<Player> GameWorld::findPlayer() const {
    for (const auto& entity : entities) {
        if (auto player = std::dynamic_pointer_cast<Player>(entity)) {
            return player;
        }
    }
    return nullptr;
}