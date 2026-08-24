#include "Graphics/AdvancedGraphicsSystem.h"
#include "World/TileMap.h"
#include "World/TileBasedLocation.h"
#include "Entities/Entity.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <memory>
#include "UISystem.h"

// Конструктор
AdvancedGraphicsSystem::AdvancedGraphicsSystem()
    : window(sf::VideoMode(1024, 768), "Eternal Wanderer", sf::Style::Default),
    uiSystem(std::make_unique<UISystem>()) {
    std::cout << "Window created with size: "
        << window.getSize().x << "x" << window.getSize().y << std::endl;
}

// Деструктор
AdvancedGraphicsSystem::~AdvancedGraphicsSystem() {
    shutdown();
}

// Инициализация системы
bool AdvancedGraphicsSystem::initialize() {
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    // Инициализация видов
    gameView.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    uiView.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    uiView.setCenter(static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2);
    // Создаем текстуры для каждого слоя
    createLayerTextures();
    // Загружаем текстуры, шейдеры и шрифты
    loadTextures();
    loadShaders();
    loadFonts();
    initialized = true;
    std::cout << "AdvancedGraphicsSystem initialized successfully." << std::endl;
    return true;
}

// Создание текстур для слоев
void AdvancedGraphicsSystem::createLayerTextures() {
    sf::Vector2u windowSize = window.getSize();
    for (int layer = static_cast<int>(RenderLayer::BACKGROUND);
        layer <= static_cast<int>(RenderLayer::UI); ++layer) {
        RenderLayer layerType = static_cast<RenderLayer>(layer);
        auto texture = std::make_unique<sf::RenderTexture>();
        if (texture->create(windowSize.x, windowSize.y)) {
            layerTextures[layerType] = std::move(texture);
            layerSprites[layerType] = sf::Sprite(layerTextures[layerType]->getTexture());
        }
        else {
            std::cerr << "Failed to create render texture for layer: " << layer << std::endl;
        }
    }
}

// Загрузка текстур
void AdvancedGraphicsSystem::loadTextures() {
    // Загрузка текстур для тайлов (БЕЗ сглаживания)
    std::vector<std::pair<std::string, std::string>> tilePaths = {
        {"ground_1", "assets/textures/tiles/base/ground_1.png"},
        {"ground_2", "assets/textures/tiles/base/ground_2.png"},
        {"ground_3", "assets/textures/tiles/base/ground_3.png"},
        {"ground_4", "assets/textures/tiles/base/ground_4.png"},
        {"water", "assets/textures/tiles/base/water.png"},
        {"desert", "assets/textures/tiles/base/desert.png"},
        {"road", "assets/textures/tiles/object/road_segment.png"},
        {"building", "assets/textures/tiles/base/building.png"},
        {"mountain", "assets/textures/tiles/base/mountain.png"},
        {"void", "assets/textures/tiles/base/void.png"},
        {"tree", "assets/textures/tiles/object/tree.png"},
        {"grass_patch", "assets/textures/tiles/decor/grass_patch.png"},
        {"house", "assets/textures/tiles/object/house.png"},
        {"stone", "assets/textures/tiles/decor/stone.png"},
        {"flower", "assets/textures/tiles/decor/flower.png"},
        {"ruin", "assets/textures/tiles/decor/ruin.png"},
        {"spawn", "assets/textures/tiles/base/spawn.png"}
    };
    for (const auto& path : tilePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path.second)) {
            texture.setSmooth(false); // КЛЮЧЕВАЯ СТРОКА ДЛЯ ТАЙЛОВ
            textureCache[path.first] = std::move(texture);
            std::cout << "Loaded texture: " << path.first << " from " << path.second << std::endl;
        }
        else {
            std::cerr << "Failed to load texture: " << path.first << " from " << path.second << std::endl;
        }
    }

    // Загрузка текстур для сущностей (БЕЗ сглаживания)
    std::vector<std::pair<std::string, std::string>> entityPaths = {
        {"player", "assets/textures/entities/player.png"},
        {"enemy", "assets/textures/entities/enemy.png"},
        {"npc", "assets/textures/entities/npc.png"},
        {"villager", "assets/textures/entities/villager.png"}
    };
    for (const auto& path : entityPaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path.second)) {
            texture.setSmooth(false); // 🔥 КЛЮЧЕВАЯ СТРОКА ДЛЯ СПРАЙТОВ
            textureCache[path.first] = std::move(texture);
            std::cout << "Loaded texture: " << path.first << " from " << path.second << std::endl;
        }
        else {
            std::cerr << "Failed to load texture: " << path.first << " from " << path.second << std::endl;
        }
    }

    // Загрузка текстур для частиц и эффектов (СО сглаживанием, если нужно)
    std::vector<std::string> effectPaths = {
        "assets/textures/effects/fire.png",
        "assets/textures/effects/smoke.png",
        "assets/textures/effects/magic.png"
    };
    for (size_t i = 0; i < effectPaths.size(); ++i) {
        sf::Texture texture;
        if (texture.loadFromFile(effectPaths[i])) {
            // Для эффектов можно оставить сглаживание (true) или отключить (false) — на ваше усмотрение
            texture.setSmooth(true); // Например, для плавных огненных частиц
            textureCache["effect_" + std::to_string(i)] = std::move(texture);
            std::cout << "Loaded effect texture: " << effectPaths[i] << std::endl;
        }
        else {
            std::cerr << "Failed to load effect texture: " << effectPaths[i] << std::endl;
        }
    }
}

// Загрузка шейдеров
void AdvancedGraphicsSystem::loadShaders() {
    // Загрузка шейдера освещения
    auto lightShader = std::make_unique<sf::Shader>();
    if (lightShader->loadFromFile("assets/shaders/lighting.vert", "assets/shaders/lighting.frag")) {
        shaderCache["lighting"] = std::move(lightShader);
        std::cout << "Lighting shader loaded successfully." << std::endl;
    }
    else {
        std::cerr << "Warning: Failed to load lighting shader!" << std::endl;
    }
    // Загрузка шейдера пост-обработки
    auto postProcessShader = std::make_unique<sf::Shader>();
    if (postProcessShader->loadFromFile("assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag")) {
        shaderCache["postprocess"] = std::move(postProcessShader);
        std::cout << "Post-process shader loaded successfully." << std::endl;
    }
    else {
        std::cerr << "Warning: Failed to load post-process shader!" << std::endl;
    }
}

// Загрузка шрифтов
void AdvancedGraphicsSystem::loadFonts() {
    // Загрузка шрифтов
    std::vector<std::pair<std::string, std::string>> fontPaths = {
        {"arial", "assets/fonts/arial.ttf"},
        {"consolas", "assets/fonts/consolas.ttf"},
        {"times", "assets/fonts/times.ttf"}
    };
    for (const auto& path : fontPaths) {
        sf::Font font;
        if (font.loadFromFile(path.second)) {
            fontCache[path.first] = std::move(font);
            std::cout << "Loaded font: " << path.first << " from " << path.second << std::endl;
        }
        else {
            std::cerr << "Failed to load font: " << path.first << " from " << path.second << std::endl;
        }
    }
}

// Завершение работы системы
void AdvancedGraphicsSystem::shutdown() {
    if (initialized) {
        window.close();
        textureCache.clear();
        shaderCache.clear();
        fontCache.clear();
        layerTextures.clear();
        layerSprites.clear();
        initialized = false;
        std::cout << "AdvancedGraphicsSystem shut down." << std::endl;
    }
}

// Начало кадра
void AdvancedGraphicsSystem::beginFrame() {
    if (!initialized) {
        std::cerr << "AdvancedGraphicsSystem not initialized!" << std::endl;
        return;
    }
    // Очищаем все слои
    for (auto& layer : layerTextures) {
        layer.second->clear(sf::Color::Transparent); // Используем -> вместо .
    }
}

// Рендеринг мира
void AdvancedGraphicsSystem::renderWorld(const std::shared_ptr<Entity>& player,
    const std::vector<std::shared_ptr<Entity>>& entities,
    const TileBasedLocation* location) {
    if (!location || !initialized) {
        std::cerr << "renderWorld: location is null or system not initialized" << std::endl;
        return;
    }
    const TileMap* tileMap = location->getTileMap();
    if (!tileMap) {
        std::cerr << "renderWorld: tileMap is null" << std::endl;
        return;
    }
    // Обновляем камеру
    if (player) {
        auto pos = player->getPosition();
        float pixelX = static_cast<float>(pos.first) * 64.0f + 32.0f;
        float pixelY = static_cast<float>(pos.second) * 64.0f + 32.0f;
        gameView.setCenter(pixelX, pixelY);
    }
    // Рендерим каждый слой
    renderBackgroundLayer();
    renderTerrainLayer(tileMap);
    renderObjectsLayer(tileMap);
    renderEntitiesLayer(entities);
    renderEffectsLayer();
}

// Рендеринг фона
void AdvancedGraphicsSystem::renderBackgroundLayer() {
    sf::RenderTexture& texture = *layerTextures[RenderLayer::BACKGROUND]; 
    texture.setView(gameView);
    sf::RectangleShape background(sf::Vector2f(40000, 40000)); 
    background.setFillColor(sf::Color(135, 206, 235)); 
    texture.draw(background);
    texture.display();
}

// Рендеринг terrain слоя
void AdvancedGraphicsSystem::renderTerrainLayer(const TileMap* tileMap) {
    // Исправлено: используем * для разыменования unique_ptr
    sf::RenderTexture& texture = *layerTextures[RenderLayer::TERRAIN];
    texture.setView(gameView);
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();
    // Получаем границы видимой области
    sf::Vector2f viewCenter = gameView.getCenter();
    sf::Vector2f viewSize = gameView.getSize();
    int startX = static_cast<int>((viewCenter.x - viewSize.x / 2) / 64.0f);
    int startY = static_cast<int>((viewCenter.y - viewSize.y / 2) / 64.0f);
    int endX = static_cast<int>((viewCenter.x + viewSize.x / 2) / 64.0f) + 1;
    int endY = static_cast<int>((viewCenter.y + viewSize.y / 2) / 64.0f) + 1;
    // Ограничиваем границы
    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(width, endX);
    endY = std::min(height, endY);

    std::cout << "Rendering terrain tiles from (" << startX << "," << startY << ") to (" << endX << "," << endY << ")" << std::endl;

    // Рендерим тайлы
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            const TileLayer& layer = tileMap->getLayer(x, y);
            TileType baseType = layer.baseType;

            std::string textureName;
            if (baseType == TileType::GROUND) {
                int variant = tileMap->getLayer(x, y).groundVariant;
                // Ограничиваем диапазон: 0–3 → ground_1 … ground_4
                variant = std::clamp(variant, 0, 3);
                textureName = "ground_" + std::to_string(variant + 1);
            }
            else {
                // Для остальных типов — как раньше
                switch (baseType) {
                case TileType::WATER: textureName = "water"; break;
                case TileType::DESERT: textureName = "desert"; break;
                case TileType::MOUNTAIN: textureName = "mountain"; break;
                case TileType::VOID: textureName = "void"; break;
                case TileType::SPAWN: textureName = "spawn"; break;
                default: textureName = "ground"; break;
                }
            }

            auto it = textureCache.find(textureName);
            if (it != textureCache.end()) {
                sf::Sprite sprite(it->second);
                sprite.setPosition(static_cast<float>(x) * 64.0f, static_cast<float>(y) * 64.0f);
                texture.draw(sprite);
            }
        }
    }
    texture.display();
}

// Рендеринг объектов слоя
void AdvancedGraphicsSystem::renderObjectsLayer(const TileMap* tileMap) {
    // Исправлено: используем * для разыменования unique_ptr
    sf::RenderTexture& texture = *layerTextures[RenderLayer::OBJECTS];
    texture.setView(gameView);
    int width = tileMap->getWidth();
    int height = tileMap->getHeight();
    // Получаем границы видимой области
    sf::Vector2f viewCenter = gameView.getCenter();
    sf::Vector2f viewSize = gameView.getSize();
    int startX = static_cast<int>((viewCenter.x - viewSize.x / 2) / 64.0f);
    int startY = static_cast<int>((viewCenter.y - viewSize.y / 2) / 64.0f);
    int endX = static_cast<int>((viewCenter.x + viewSize.x / 2) / 64.0f) + 1;
    int endY = static_cast<int>((viewCenter.y + viewSize.y / 2) / 64.0f) + 1;
    // Ограничиваем границы
    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(width, endX);
    endY = std::min(height, endY);
    // Рендерим объекты
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            const TileLayer& layer = tileMap->getLayer(x, y);
            // Рендерим объект
            if (layer.objectType != TileType::VOID) {
                std::string textureName;
                switch (layer.objectType) {
                case TileType::TREE: textureName = "tree"; break;
                case TileType::HOUSE: textureName = "house"; break;
                case TileType::ROAD_SEGMENT: textureName = "road"; break;
                case TileType::MOUNTAIN: textureName = "mountain"; break;
                case TileType::BUILDING: textureName = "building"; break;
                case TileType::RUIN: textureName = "ruin"; break;
                default: textureName = "ground"; break;
                }
                auto it = textureCache.find(textureName);
                if (it != textureCache.end()) {
                    sf::Sprite sprite(it->second);
                    sprite.setPosition(static_cast<float>(x) * 64.0f, static_cast<float>(y) * 64.0f);
                    texture.draw(sprite);
                }
            }
            // Рендерим декорации
            for (const auto& decType : layer.decorationLayers) {
                std::string textureName;
                switch (decType) {
                case TileType::GRASS_PATCH: textureName = "grass_patch"; break;
                case TileType::STONE: textureName = "stone"; break;
                case TileType::FLOWER: textureName = "flower"; break;
                default: continue;
                }
                auto it = textureCache.find(textureName);
                if (it != textureCache.end()) {
                    sf::Sprite sprite(it->second);
                    sprite.setPosition(static_cast<float>(x) * 64.0f, static_cast<float>(y) * 64.0f);
                    texture.draw(sprite);
                }
            }
        }
    }
    texture.display();
}

// Рендеринг сущностей
void AdvancedGraphicsSystem::renderEntitiesLayer(const std::vector<std::shared_ptr<Entity>>& entities) {
    // Исправлено: используем * для разыменования unique_ptr
    sf::RenderTexture& texture = *layerTextures[RenderLayer::ENTITIES];
    texture.setView(gameView);
    for (const auto& entity : entities) {
        if (!entity) continue;
        auto pos = entity->getPosition();
        float entityX = static_cast<float>(pos.first) * 64.0f;
        float entityY = static_cast<float>(pos.second) * 64.0f;
        std::string textureName;
        if (entity->getType() == "player") {
            textureName = "player";
        }
        else if (entity->getType() == "enemy") {
            textureName = "enemy";
        }
        else if (entity->getType() == "npc") {
            textureName = "npc";
        }
        else if (entity->getType() == "villager") {
            textureName = "villager";
        }
        else {
            textureName = "npc";
        }
        auto it = textureCache.find(textureName);
        if (it != textureCache.end()) {
            sf::Sprite sprite(it->second);
            sprite.setPosition(entityX, entityY);
            texture.draw(sprite);
        }
    }
    texture.display();
}

// Рендеринг эффектов
void AdvancedGraphicsSystem::renderEffectsLayer() {
    // Исправлено: используем * для разыменования unique_ptr
    sf::RenderTexture& texture = *layerTextures[RenderLayer::EFFECTS];
    texture.setView(gameView);
    // Здесь будет рендер частиц и других эффектов
    // Для примера рисуем несколько частиц
    for (int i = 0; i < 10; ++i) {
        float x = gameView.getCenter().x + (rand() % 200 - 100);
        float y = gameView.getCenter().y + (rand() % 200 - 100);
        auto it = textureCache.find("effect_0");
        if (it != textureCache.end()) {
            sf::Sprite sprite(it->second);
            sprite.setPosition(x, y);
            sprite.setColor(sf::Color(255, 100, 0, 150));
            texture.draw(sprite);
        }
    }
    texture.display();
}

// Конец кадра
void AdvancedGraphicsSystem::endFrame() {
    if (!initialized) {
        std::cerr << "AdvancedGraphicsSystem not initialized!" << std::endl;
        return;
    }
    // Создаем временную текстуру для пост-обработки
    sf::RenderTexture tempTexture;
    if (!tempTexture.create(window.getSize().x, window.getSize().y)) {
        std::cerr << "Failed to create temp texture for post-processing!" << std::endl;
        return;
    }
    tempTexture.clear(sf::Color::Transparent);
    // Рисуем все слои
    for (int layer = static_cast<int>(RenderLayer::BACKGROUND);
        layer <= static_cast<int>(RenderLayer::EFFECTS); ++layer) {
        RenderLayer layerType = static_cast<RenderLayer>(layer);
        if (layerTextures.find(layerType) != layerTextures.end()) {
            sf::Sprite sprite(layerTextures[layerType]->getTexture());
            tempTexture.draw(sprite);
        }
    }
    tempTexture.display();
    // Применяем пост-обработку
    window.draw(sf::Sprite(tempTexture.getTexture()));
    window.display();
}

// Обработка событий
void AdvancedGraphicsSystem::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        // Добавить обработку других событий
    }
}

// Проверка, открыто ли окно
bool AdvancedGraphicsSystem::isWindowOpen() const {
    return window.isOpen();
}

// Получение окна
sf::RenderWindow& AdvancedGraphicsSystem::getWindow() {
    return window;
}

// Получение игрового вида
sf::View& AdvancedGraphicsSystem::getGameView() {
    return gameView;
}

// Установка позиции камеры
void AdvancedGraphicsSystem::setCameraPosition(float x, float y) {
    gameView.setCenter(x, y);
}

// Установка зума камеры
void AdvancedGraphicsSystem::setCameraZoom(float zoom) {
    gameView.zoom(zoom);
}

// Установка вращения камеры
void AdvancedGraphicsSystem::setCameraRotation(float rotation) {
    gameView.setRotation(rotation);
}

// Получение текстуры
sf::Texture* AdvancedGraphicsSystem::getTexture(const std::string& name) {
    auto it = textureCache.find(name);
    if (it != textureCache.end()) {
        return &it->second;
    }
    return nullptr;
}

// Загрузка текстуры
bool AdvancedGraphicsSystem::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        texture.setSmooth(false); 
        textureCache[name] = std::move(texture);
        return true;
    }
    return false;
}

// Получение шейдера
sf::Shader* AdvancedGraphicsSystem::getShader(const std::string& name) {
    auto it = shaderCache.find(name);
    if (it != shaderCache.end() && it->second) {
        return it->second.get();
    }
    return nullptr;
}

// Загрузка шейдера
bool AdvancedGraphicsSystem::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    auto shader = std::make_unique<sf::Shader>();
    if (shader->loadFromFile(vertexPath, fragmentPath)) {
        shaderCache[name] = std::move(shader);
        return true;
    }
    return false;
}

// Получение шрифта
sf::Font* AdvancedGraphicsSystem::getFont(const std::string& name) {
    auto it = fontCache.find(name);
    if (it != fontCache.end()) {
        return &it->second;
    }
    return nullptr;
}

// Загрузка шрифта
bool AdvancedGraphicsSystem::loadFont(const std::string& name, const std::string& path) {
    sf::Font font;
    if (font.loadFromFile(path)) {
        fontCache[name] = std::move(font);
        return true;
    }
    return false;
}

// Добавление частиц
void AdvancedGraphicsSystem::addParticleEffect(const sf::Vector2f& position, const std::string& effectType, int count) {
    // Здесь будет реализация добавления частиц
    // Для примера просто выводим сообщение
    std::cout << "Added particle effect at " << position.x << ", " << position.y
        << " of type " << effectType << " with count " << count << std::endl;
}

// Обновление частиц
void AdvancedGraphicsSystem::updateParticleEffects(float deltaTime) {
    // Здесь будет реализация обновления частиц
}

// Применение освещения
void AdvancedGraphicsSystem::applyLighting() {
    if (shaderCache.find("lighting") == shaderCache.end() || !shaderCache["lighting"]) return;
    sf::RenderTexture tempTexture;
    if (!tempTexture.create(window.getSize().x, window.getSize().y)) {
        std::cerr << "Failed to create temp texture for lighting!" << std::endl;
        return;
    }
    tempTexture.clear(sf::Color::Transparent);
    tempTexture.setView(gameView);
    // Исправлено: используем -> для доступа к уникальному указателю
    sf::Sprite worldSprite(layerTextures[RenderLayer::TERRAIN]->getTexture());
    worldSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
    tempTexture.draw(worldSprite, shaderCache["lighting"].get());
    tempTexture.display();
    // Копируем результат обратно в worldTexture
    layerTextures[RenderLayer::TERRAIN]->clear(sf::Color::Transparent);
    sf::Sprite tempSprite(tempTexture.getTexture());
    layerTextures[RenderLayer::TERRAIN]->draw(tempSprite);
    layerTextures[RenderLayer::TERRAIN]->display();
}

// Применение пост-обработки
void AdvancedGraphicsSystem::applyPostProcessing() {
    if (shaderCache.find("postprocess") == shaderCache.end() || !shaderCache["postprocess"]) return;
    sf::RenderTexture tempTexture;
    if (!tempTexture.create(window.getSize().x, window.getSize().y)) {
        std::cerr << "Failed to create temp texture for post-processing!" << std::endl;
        return;
    }
    tempTexture.clear(sf::Color::Transparent);
    tempTexture.setView(uiView);
    // Исправлено: используем -> для доступа к уникальному указателю
    for (int layer = static_cast<int>(RenderLayer::TERRAIN
        );
        layer <= static_cast<int>(RenderLayer::UI); ++layer) {
        RenderLayer layerType = static_cast<RenderLayer>(layer);
        if (layerTextures.find(layerType) != layerTextures.end()) {
            sf::Sprite sprite(layerTextures[layerType]->getTexture());
            sprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
            tempTexture.draw(sprite);
        }
    }
    tempTexture.display();
    // Копируем результат обратно в финальную текстуру
    layerTextures[RenderLayer::UI]->clear(sf::Color::Transparent);
    sf::Sprite tempSprite(tempTexture.getTexture());
    layerTextures[RenderLayer::UI]->draw(tempSprite);
    layerTextures[RenderLayer::UI]->display();
}