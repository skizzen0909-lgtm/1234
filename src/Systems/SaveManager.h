// Systems/SaveManager.h
#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Forward declarations
class Entity;
class GameWorld;
class Quest;

// Структуры для сохранения данных
struct PlayerSaveData {
    std::string name;
    int health;
    int maxHealth;
    int level;
    int experience;
    std::vector<std::string> inventoryItems; // Только ID предметов
    std::vector<std::string> activeQuests;   // Только ID квестов
    std::string currentLocation;

    PlayerSaveData() : health(0), maxHealth(0), level(0), experience(0) {}
};

struct WorldSaveData {
    std::string worldName;
    std::vector<std::string> locationNames;
    int currentLocationIndex;

    WorldSaveData() : currentLocationIndex(0) {}
};

class SaveManager {
private:
    std::string saveDirectory;
    int saveVersion;

public:
    // Конструктор
    explicit SaveManager(const std::string& saveDir = "./saves/");

    // Деструктор
    ~SaveManager() = default;

    // Методы сохранения
    bool saveGame(const Entity* player, const GameWorld* world, const std::string& filename);
    bool loadGame(Entity* player, GameWorld* world, const std::string& filename);

    // Методы работы с файлами
    bool saveToFile(const std::string& filename, const std::string& data) const;
    bool loadFromFile(const std::string& filename, std::string& data) const;

    // Методы сериализации
    std::string serializePlayerData(const Entity* player) const;
    bool deserializePlayerData(Entity* player, const std::string& data) const;

    std::string serializeWorldData(const GameWorld* world) const;
    bool deserializeWorldData(GameWorld* world, const std::string& data) const;

    // Утилиты
    std::string getFullSavePath(const std::string& filename) const;
    bool createSaveDirectory() const;
    bool isValidSaveFile(const std::string& filename) const;

    // Запрет копирования
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;
};

#endif // SAVEMANAGER_H