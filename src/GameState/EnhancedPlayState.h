// GameState/EnhancedPlayState.h
#pragma once

#include "GameState.h"
#include "Entities/Player.h"
#include "World/GameWorld.h"
#include "Graphics/UISystem.h"
#include "Utils/PerlinNoise.h"
#include "World/TileMap.h"
#include <memory>
#include <vector>
#include <unordered_map>

// Forward declarations
class AIBase;

class EnhancedPlayState : public GameState {
private:
    std::unordered_map<std::string, std::unique_ptr<AIBase>> aiBehaviors;
    std::shared_ptr<Player> player;
    std::unique_ptr<GameWorld> gameWorld;
    std::unique_ptr<UISystem> uiSystem;
    float timeAccumulator = 0.0f;
    bool paused = false;

    void initializeAI();          
    void handlePlayerInput();    
   
    void expandWorld(TileBasedLocation* location, std::pair<int, int> playerPos);
    void generateTerrainForTile(TileMap* tileMap, int x, int y);
    std::unique_ptr<PerlinNoise> baseNoise;
public:
    EnhancedPlayState();
    ~EnhancedPlayState() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void handleInput() override;
    void render() override;
    void togglePause();
    void showInventory();
    void showSkills();
    void showMap();
};