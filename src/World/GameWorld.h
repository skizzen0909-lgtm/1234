// World/GameWorld.h
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Location.h"
#include <vector>
#include <memory>
#include <string>
#include "Entities/Player.h"

class GameWorld {
private:
    std::string worldName;
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Location>> locations;
    int currentLocationIndex;
public:
    explicit GameWorld(const std::string& worldName);
    ~GameWorld() = default;

    // Управление сущностями
    bool addEntity(std::shared_ptr<Entity> entity);
    bool removeEntity(const std::shared_ptr<Entity>& entity);
    const std::vector<std::shared_ptr<Entity>>& getEntities() const;

    // Управление локациями
    void addLocation(std::unique_ptr<Location> location);
    Location* getCurrentLocation() const;
    void setCurrentLocation(int locationIndex);
    bool changeLocation(int locationIndex);

    // Геттеры
    const std::vector<std::unique_ptr<Location>>& getLocations() const;
    const std::string& getWorldName() const;
    void setWorldName(const std::string& name);
    int getCurrentLocationIndex() const;

    // Обновление мира
    void update(float deltaTime);

    // Поиск игрока
    std::shared_ptr<Player> findPlayer() const;

    // Запрет копирования
    GameWorld(const GameWorld&) = delete;
    GameWorld& operator=(const GameWorld&) = delete;
};

#endif // GAMEWORLD_H