// Graphics/AdvancedGraphicsSystem.h
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "Types/Types.h"
#include "UISystem.h"

// Forward declarations
class Entity;
class TileBasedLocation;
class TileMap;

// Определение слоев рендеринга
enum class RenderLayer {
    BACKGROUND,
    TERRAIN,
    OBJECTS,
    ENTITIES,
    EFFECTS,
    UI
};

// Класс для управления рендерингом
class AdvancedGraphicsSystem {
private:
    sf::RenderWindow window;
    std::unordered_map<RenderLayer, std::unique_ptr<sf::RenderTexture>> layerTextures;
    std::unordered_map<RenderLayer, sf::Sprite> layerSprites;
    std::unordered_map<std::string, sf::Texture> textureCache;
    std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaderCache; // Используем unique_ptr
    std::unordered_map<std::string, sf::Font> fontCache;
    sf::View gameView;
    sf::View uiView;
    bool initialized = false;
    float time = 0.0f;
    // Добавляем член для UI системы
    std::unique_ptr<UISystem> uiSystem;
    // --- Методы для загрузки ресурсов ---
    void loadTextures();
    void loadShaders();
    void loadFonts();
    void createLayerTextures();
    // Методы для рендеринга каждого слоя
    void renderBackgroundLayer();
    void renderTerrainLayer(const TileMap* tileMap);
    void renderObjectsLayer(const TileMap* tileMap);
    void renderEntitiesLayer(const std::vector<std::shared_ptr<Entity>>& entities);
    void renderEffectsLayer();
    
public:
    AdvancedGraphicsSystem();
    ~AdvancedGraphicsSystem();
    bool initialize();
    void shutdown();
    void beginFrame();
    void renderWorld(const std::shared_ptr<Entity>& player,
        const std::vector<std::shared_ptr<Entity>>& entities,
        const TileBasedLocation* location);
    void endFrame();
    void handleEvents();
    bool isWindowOpen() const;
    sf::RenderWindow& getWindow();
    sf::View& getGameView();
    // Методы для работы с камерой
    void setCameraPosition(float x, float y);
    void setCameraZoom(float zoom);
    void setCameraRotation(float rotation);
    // Методы для работы с текстурами
    sf::Texture* getTexture(const std::string& name);
    bool loadTexture(const std::string& name, const std::string& path);
    // Методы для работы с шейдерами
    sf::Shader* getShader(const std::string& name);
    bool loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    // Методы для работы с шрифтами
    sf::Font* getFont(const std::string& name);
    bool loadFont(const std::string& name, const std::string& path);
    // Методы для работы с эффектами
    void addParticleEffect(const sf::Vector2f& position, const std::string& effectType, int count);
    void updateParticleEffects(float deltaTime);
    // Применение шейдеров
    void applyLighting();
    void applyPostProcessing();
};