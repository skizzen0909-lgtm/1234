// Types/Types.h
#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <vector>

enum class TileType {
    GROUND,     // Основание (земля)
    WATER,      // Основание (непроходимое)
    DESERT,     // Основание (пустыня)
    // --- НОВЫЕ ТИПЫ ---
    GRASS,      // Основание (лес, равнина)
    SAND,       // Основание (пляж, пустыня)
    SNOW,       // Основание (тайга, тундра)
    ROCK,       // Основание (горы)
    // ---
    // - ОБЪЕКТЫ (наложенные поверх основания GROUND) -
    TREE,       // Объект на земле
    ROAD_SEGMENT, // Объект на земле
    HOUSE,      // Объект на земле
    MOUNTAIN,   // Объект на земле (всегда поверх GROUND)
    BUILDING,   // Объект на земле (всегда поверх GROUND)
    STONE,      // Объект на земле
    FLOWER,     // Объект на земле
    RUIN,       // Объект на земле
    // - ДЕКОРАЦИИ (не влияют на проходимость, только на GROUND) -
    GRASS_PATCH, // Декорация на земле
    // технические
    VOID,
    SPAWN
};

// Константы путей к спрайтам тайлов (для GraphicsSystem)
namespace TileSpritePaths {
    constexpr const char* GROUND = "assets/textures/tiles/base/ground.png";
    constexpr const char* WATER = "assets/textures/tiles/base/water.png";
    constexpr const char* DESERT = "assets/textures/tiles/base/desert.png";
    constexpr const char* ROAD = "assets/textures/tiles/object/road_segment.png";
    constexpr const char* BUILDING = "assets/textures/tiles/base/building.png";
    constexpr const char* MOUNTAIN = "assets/textures/tiles/base/mountain.png";
    constexpr const char* VOID = "assets/textures/tiles/base/void.png";
    constexpr const char* TREE = "assets/textures/tiles/object/tree.png";
    constexpr const char* GRASS_PATCH = "assets/textures/tiles/decor/grass_patch.png";
    constexpr const char* HOUSE = "assets/textures/tiles/object/house.png";
    constexpr const char* STONE = "assets/textures/tiles/decor/stone.png";
    constexpr const char* FLOWER = "assets/textures/tiles/decor/flower.png";
    constexpr const char* RUIN = "assets/textures/tiles/decor/ruin.png";
    constexpr const char* SPAWN = "assets/textures/tiles/base/spawn.png";
    // Добавляем структуру для использования в AdvancedGraphicsSystem.cpp
    static const std::vector<std::pair<TileType, const char*>> tilePaths = {
        {TileType::GROUND, GROUND},
        {TileType::WATER, WATER},
        {TileType::DESERT, DESERT},
        {TileType::ROAD_SEGMENT, ROAD},
        {TileType::BUILDING, BUILDING},
        {TileType::MOUNTAIN, MOUNTAIN},
        {TileType::VOID, VOID},
        {TileType::TREE, TREE},
        {TileType::GRASS_PATCH, GRASS_PATCH},
        {TileType::HOUSE, HOUSE},
        {TileType::STONE, STONE},
        {TileType::FLOWER, FLOWER},
        {TileType::RUIN, RUIN},
        {TileType::SPAWN, SPAWN}
    };
}

enum class EntityType {
    PLAYER,
    NPC,
    ENEMY,
    VILLAGER
};

enum class EventType {
    COMBAT,
    DIALOGUE,
    QUEST,
    STORY,
    PEACEFUL_ACTIVITY
};

enum class GameStateType {
    MENU,
    PLAY,
    PAUSE,
    GAME_OVER,
    SETTINGS
};

enum class QuestStatus {
    NOT_STARTED,
    IN_PROGRESS,
    COMPLETED,
    FAILED
};

struct Position {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    Position() = default;
    Position(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Stats {
    int strength = 10;
    int agility = 10;
    int intelligence = 10;
    int constitution = 10;
    Stats() = default;
    Stats(int str, int agi, int intel, int con)
        : strength(str), agility(agi), intelligence(intel), constitution(con) {
    }
};

namespace Constants {
    constexpr int MAX_INVENTORY_SIZE = 20;
    constexpr int MAX_LEVEL = 50;
    constexpr float BASE_EXPERIENCE_MULTIPLIER = 1.0f;
    constexpr int STARTING_HEALTH = 100;
}

namespace std {
    template<>
    struct hash<TileType> {
        size_t operator()(const TileType& t) const noexcept {
            return static_cast<size_t>(t);
        }
    };
}

#endif // TYPES_H