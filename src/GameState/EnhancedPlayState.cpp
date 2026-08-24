// src/GameState/EnhancedPlayState.cpp
#include "GameState/EnhancedPlayState.h"
#include "AI/AdvancedAI.h"
#include "Core/Engine.h"
#include "Events/CombatEvent.h"
#include "Events/EventManager.h"
#include "Graphics/AdvancedGraphicsSystem.h"
#include "Graphics/UISystem.h"
#include "World/AdvancedWorldGenerator.h"
#include "World/TileBasedLocation.h"
#include "World/TileMap.h"
#include <iostream>
#include <memory>

EnhancedPlayState::EnhancedPlayState() : GameState("enhanced_play"), baseNoise(std::make_unique<PerlinNoise>(0)) {
}

void EnhancedPlayState::enter() {
    Engine* engineInstance = Engine::getInstance();
    if (!engineInstance) {
        std::cerr << "Graphics system not available!" << std::endl;
        return;
    }
    auto* graphicsSystem = engineInstance->getGraphicsSystem();
    if (!graphicsSystem) {
        std::cerr << "Graphics system is not available!" << std::endl;
        return;
    }
    if (!graphicsSystem->getWindow().isOpen()) {
        std::cerr << "Graphics window is not open!" << std::endl;
        return;
    }

    uiSystem = std::make_unique<UISystem>();
    if (!uiSystem->initialize(graphicsSystem->getWindow())) {
        std::cerr << "Failed to initialize UI system!" << std::endl;
        return;
    }

    AdvancedWorldGenerator generator;
    AdvancedWorldGenerator::GenerationParams params;
    params.worldSize = 512;
    params.regionSize = 64;
    params.mountainHeight = 0.8f;
    params.waterLevel = 0.4f;
    params.forestThreshold = 0.3f;
    generator.setParams(params);

    gameWorld = generator.generateWorld("Мир Вечного Странника");
    std::cout << "gameWorld created: " << (gameWorld ? "yes" : "no") << std::endl;
    if (gameWorld) {
        std::cout << "Locations count: " << gameWorld->getLocations().size() << std::endl;
        std::cout << "Current location index: " << gameWorld->getCurrentLocationIndex() << std::endl;
        auto* loc = gameWorld->getCurrentLocation();
        std::cout << "Current location ptr: " << loc << std::endl;
        if (loc) {
            std::cout << "Location name: " << loc->getName() << std::endl;
            auto* tileLoc = dynamic_cast<TileBasedLocation*>(loc);
            std::cout << "Is TileBasedLocation: " << (tileLoc ? "yes" : "no") << std::endl;
            if (tileLoc && tileLoc->getTileMap()) {
                std::cout << "TileMap size: " << tileLoc->getTileMap()->getWidth() << "x" << tileLoc->getTileMap()->getHeight() << std::endl;
            }
        }
    }
    if (!gameWorld) {
        uiSystem->showMessage("Ошибка генерации мира!");
        return;
    }
    std::cout << "Локаций в мире: " << gameWorld->getLocations().size() << std::endl;
    std::cout << "Текущая локация индекс: " << gameWorld->getCurrentLocationIndex() << std::endl;

    player = std::make_shared<Player>("Hero", 100);

    AIPersonality playerPersonality;
    playerPersonality.curiosity = 0.9f;   // Игрок любопытен — исследует мир
    playerPersonality.aggression = 0.4f;  // Умеренная агрессия
    playerPersonality.sociability = 0.6f; // Любит общаться
    playerPersonality.caution = 0.7f;     // Осторожен — убегает от сильных врагов
    player->setAI(std::make_unique<AdvancedAI>(playerPersonality));
    std::cout << "Player AI set in EnhancedPlayState: " << (player->getAI() != nullptr) << std::endl;

    if (auto* location = gameWorld->getCurrentLocation()) {
        if (auto tileLoc = dynamic_cast<TileBasedLocation*>(location)) {
            auto spawnPos = tileLoc->findSpawnPoint();
            if (spawnPos.first != -1 && spawnPos.second != -1) {
                player->setPosition(spawnPos.first, spawnPos.second);
                player->setCurrentLocation(location);
                location->addEntity(player);
                gameWorld->addEntity(player);
                std::cout << "Игрок 'Hero' создан и добавлен в локацию в (" << spawnPos.first << ", " << spawnPos.second << ")." << std::endl;
            }
            else {
                int centerX = tileLoc->getTileMap()->getWidth() / 2;
                int centerY = tileLoc->getTileMap()->getHeight() / 2;
                if (tileLoc->getTileMap()->isWalkable(centerX, centerY)) {
                    player->setPosition(centerX, centerY);
                    player->setCurrentLocation(location);
                    location->addEntity(player);
                    std::cerr << "FALLBACK: Игрок заспавнен в центре карты (" << centerX << ", " << centerY << ")." << std::endl;
                }
            }
        }
    }

    initializeAI();

    // === ДОБАВЛЕНИЕ ЦЕЛЕЙ ДЛЯ ИГРОКА ===
    if (player && player->getAI()) {
        AdvancedAI* playerAI = dynamic_cast<AdvancedAI*>(player->getAI());
        if (playerAI && playerAI->getGoalSystem()) {
            auto* goalSystem = playerAI->getGoalSystem();

            // Цель 1: Исцелиться при низком здоровье (< 30%)
            Goal healGoal;
            healGoal.id = "heal_when_low";
            healGoal.priority = 1.0f;
            healGoal.condition = [](Entity* e) {
                return e && e->getHealth() < e->getMaxHealth() * 0.3f;
                };
            healGoal.action = [](Entity* e) {
                if (auto* ai = dynamic_cast<AdvancedAI*>(e->getAI())) {
                    ai->setState(AIState::SOCIALIZE);
                    std::cout << e->getName() << " ищет способ исцелиться!" << std::endl;
                }
                };
            goalSystem->addGoal(healGoal);

            // Цель 2: Избегать сильных врагов
            Goal avoidStrongEnemies;
            avoidStrongEnemies.id = "avoid_strong_enemies";
            avoidStrongEnemies.priority = 0.9f;
            avoidStrongEnemies.condition = [this](Entity* e) {
                if (!e || !this->gameWorld) return false;
                auto* ai = e->getAI();
                if (!ai) return false;
                if (auto* advancedAI = dynamic_cast<AdvancedAI*>(ai)) {
                    const auto& memories = advancedAI->getMemorySystem().getMemories();
                    for (const auto& mem : memories) {
                        if ((mem.eventType == "seen" || mem.eventType == "attacked_by") && mem.entity.lock()) {
                            auto enemy = mem.entity.lock();
                            if (!enemy || !enemy->isAlive()) continue;
                            int playerLevel = e->getLevel();
                            int enemyLevel = enemy->getLevel();
                            if (enemyLevel > playerLevel * 1.5f) {
                                return true;
                            }
                        }
                    }
                }
                return false;
                };
            avoidStrongEnemies.action = [](Entity* e) {
                if (auto* ai = dynamic_cast<AdvancedAI*>(e->getAI())) {
                    ai->setState(AIState::FLEE);
                    std::cout << e->getName() << " убегает от сильного врага!" << std::endl;
                }
                };
            goalSystem->addGoal(avoidStrongEnemies);

            // Цель 3: Прокачка — искать врагов своего уровня
            Goal seekLevelAppropriateEnemies;
            seekLevelAppropriateEnemies.id = "seek_training_targets";
            seekLevelAppropriateEnemies.priority = 0.7f;
            seekLevelAppropriateEnemies.condition = [](Entity* e) {
                if (auto* player = dynamic_cast<Player*>(e)) {
                    return player->getExperience() < player->getExperienceToNextLevel() * 0.9f;
                }
                return false;
                };
            seekLevelAppropriateEnemies.action = [](Entity* e) {
                if (auto* ai = dynamic_cast<AdvancedAI*>(e->getAI())) {
                    ai->setState(AIState::WANDER);
                    std::cout << e->getName() << " ищет врагов для прокачки!" << std::endl;
                }
                };
            goalSystem->addGoal(seekLevelAppropriateEnemies);

            // Цель 4: Исследование мира
            Goal exploreWorld;
            exploreWorld.id = "explore_world";
            exploreWorld.priority = 0.5f;
            exploreWorld.condition = [this](Entity* e) {
                if (!e || !this->gameWorld) return false;
                auto* loc = this->gameWorld->getCurrentLocation();
                if (!loc) return false;
                auto* tileLoc = dynamic_cast<TileBasedLocation*>(loc);
                if (!tileLoc || !tileLoc->getTileMap()) return false;
                auto [x, y] = e->getPosition();
                int worldSize = tileLoc->getTileMap()->getWidth();
                int viewRadius = 200;
                return (x < viewRadius || x > worldSize - viewRadius ||
                    y < viewRadius || y > worldSize - viewRadius);
                };
            exploreWorld.action = [](Entity* e) {
                if (auto* ai = dynamic_cast<AdvancedAI*>(e->getAI())) {
                    ai->setState(AIState::WANDER);
                    std::cout << e->getName() << " исследует границы мира!" << std::endl;
                }
                };
            goalSystem->addGoal(exploreWorld);
        }
    }

    uiSystem->showDialog("greeting", player.get(), nullptr);
    uiSystem->showMessage("Добро пожаловать в Мир Вечного Странника!");
}

void EnhancedPlayState::exit() {
    if (uiSystem) {
        uiSystem->shutdown();
    }
    gameWorld.reset();
    player.reset();
}

void EnhancedPlayState::update(float deltaTime) {
    if (paused) return;
    timeAccumulator += deltaTime;

    if (gameWorld) {
        gameWorld->update(deltaTime);
    }


    if (uiSystem) {
        uiSystem->update(deltaTime);
    }

    // Динамическая загрузка карты
    if (player && gameWorld) {
        if (auto* location = gameWorld->getCurrentLocation()) {
            if (auto tileLoc = dynamic_cast<TileBasedLocation*>(location)) {
                auto playerPos = player->getPosition();
                int viewRadius = 256;
                int worldSize = tileLoc->getTileMap()->getWidth();

                if (playerPos.first < viewRadius ||
                    playerPos.first > worldSize - viewRadius ||
                    playerPos.second < viewRadius ||
                    playerPos.second > worldSize - viewRadius) {
                    expandWorld(tileLoc, playerPos);
                }
            }
        }
    }
}

void EnhancedPlayState::expandWorld(TileBasedLocation* location, std::pair<int, int> playerPos) {
    int currentWidth = location->getTileMap()->getWidth();
    int currentHeight = location->getTileMap()->getHeight();
    int expansionSize = 256;

    auto newMap = std::make_unique<TileMap>(currentWidth + expansionSize * 2, currentHeight + expansionSize * 2);

    for (int y = 0; y < currentHeight; ++y) {
        for (int x = 0; x < currentWidth; ++x) {
            TileType type = location->getTileMap()->getTile(x, y);
            newMap->setTile(x + expansionSize, y + expansionSize, type);

            const auto& layer = location->getTileMap()->getLayer(x, y);
            if (layer.objectType != TileType::VOID) {
                newMap->addObjectToTile(x + expansionSize, y + expansionSize, layer.objectType);
            }
            for (auto dec : layer.decorationLayers) {
                newMap->addDecorationToTile(x + expansionSize, y + expansionSize, dec);
            }
        }
    }

    // Генерация новых тайлов (упрощённо)
    for (int y = 0; y < newMap->getHeight(); ++y) {
        for (int x = 0; x < newMap->getWidth(); ++x) {
            if (newMap->getTile(x, y) == TileType::VOID) {
                generateTerrainForTile(newMap.get(), x, y);
            }
        }
    }

    location->setTileMap(std::move(newMap));

    for (auto& entity : location->getEntities()) {
        auto pos = entity->getPosition();
        entity->setPosition(pos.first + expansionSize, pos.second + expansionSize);
    }
}

void EnhancedPlayState::generateTerrainForTile(TileMap* tileMap, int x, int y) {
    float nx = static_cast<float>(x) * 0.01f;
    float ny = static_cast<float>(y) * 0.01f;
    float noiseValue = baseNoise->fractalNoise(nx, ny, 5, 0.5f);

    if (noiseValue < 0.4f) {
        tileMap->setTile(x, y, TileType::WATER);
    }
    else {
        tileMap->setTile(x, y, TileType::GROUND);
    }
}

void EnhancedPlayState::initializeAI() {
    AIPersonality enemyPersonality;
    enemyPersonality.aggression = 0.9f;
    enemyPersonality.curiosity = 0.1f;
    enemyPersonality.sociability = 0.1f;
    enemyPersonality.caution = 0.7f;

    auto enemyAI = std::make_unique<AdvancedAI>(enemyPersonality);
    aiBehaviors["enemy"] = std::move(enemyAI);
}

void EnhancedPlayState::render() {
    auto* engine = Engine::getInstance();
    if (!engine) return;
    auto* graphics = engine->getGraphicsSystem();
    if (!graphics) return;

    if (gameWorld && player) {
        if (auto* location = gameWorld->getCurrentLocation()) {
            if (auto tileLoc = dynamic_cast<TileBasedLocation*>(location)) {
                std::vector<std::shared_ptr<Entity>> entities;
                for (const auto& ent : tileLoc->getEntities()) {
                    entities.push_back(ent);
                }
                graphics->renderWorld(player, entities, tileLoc);
            }
        }
    }
}

void EnhancedPlayState::handleInput() {
    handlePlayerInput();
}

void EnhancedPlayState::handlePlayerInput() {
    // Пока заглушка
}

void EnhancedPlayState::togglePause() {
    paused = !paused;
}

void EnhancedPlayState::showInventory() {
    if (uiSystem) {
        uiSystem->showInventory();
    }
}

void EnhancedPlayState::showSkills() {
    if (uiSystem) {
        uiSystem->showMessage("Система навыков в разработке...");
    }
}

void EnhancedPlayState::showMap() {
    if (uiSystem) {
        uiSystem->showMessage("Карта мира в разработке...");
    }
}

EnhancedPlayState::~EnhancedPlayState() = default;