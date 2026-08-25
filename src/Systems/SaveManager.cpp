#include "SaveManager.h"
#include "Entities/Entity.h"
#include "World/GameWorld.h"
#include "Quests/Quest.h"
#include "Inventory/Inventory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

SaveManager::SaveManager(const std::string& saveDir)
    : saveDirectory(saveDir), saveVersion(1) {
    createSaveDirectory();
}

bool SaveManager::saveGame(const Entity* player, const GameWorld* world, const std::string& filename) {
    if (!player || !world) {
        std::cerr << "Ошибка: Невозможно сохранить игру с нулевыми объектами!" << std::endl;
        return false;
    }

    try {
        // Сериализуем данные
        std::string playerData = serializePlayerData(player);
        std::string worldData = serializeWorldData(world);

        // Комбинируем данные
        std::ostringstream saveStream;
        saveStream << "SAVE_VERSION:" << saveVersion << "\n";
        saveStream << "PLAYER_DATA:" << playerData << "\n";
        saveStream << "WORLD_DATA:" << worldData << "\n";

        // Сохраняем в файл
        return saveToFile(filename, saveStream.str());

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении игры: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(Entity* player, GameWorld* world, const std::string& filename) {
    if (!player || !world) {
        std::cerr << "Ошибка: Невозможно загрузить игру в нулевые объекты!" << std::endl;
        return false;
    }

    try {
        // Загружаем данные из файла
        std::string data;
        if (!loadFromFile(filename, data)) {
            return false;
        }

        // Проверяем версию сохранения
        if (!isValidSaveFile(data)) {
            std::cerr << "Ошибка: Несовместимая версия файла сохранения!" << std::endl;
            return false;
        }

        // Разбираем данные
        std::istringstream dataStream(data);
        std::string line;

        while (std::getline(dataStream, line)) {
            if (line.find("PLAYER_DATA:") == 0) {
                std::string playerData = line.substr(12); // Убираем "PLAYER_DATA:"
                if (!deserializePlayerData(player, playerData)) {
                    return false;
                }
            }
            else if (line.find("WORLD_DATA:") == 0) {
                std::string worldData = line.substr(11); // Убираем "WORLD_DATA:"
                if (!deserializeWorldData(world, worldData)) {
                    return false;
                }
            }
        }

        std::cout << "Игра успешно загружена из " << filename << std::endl;
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке игры: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveToFile(const std::string& filename, const std::string& data) const {
    std::string fullPath = getFullSavePath(filename);

    try {
        std::ofstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Ошибка: Невозможно открыть файл для записи: " << fullPath << std::endl;
            return false;
        }

        file << data;
        file.close();

        std::cout << "Игра сохранена в " << fullPath << std::endl;
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при записи в файл: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadFromFile(const std::string& filename, std::string& data) const {
    std::string fullPath = getFullSavePath(filename);

    try {
        std::ifstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Ошибка: Невозможно открыть файл для чтения: " << fullPath << std::endl;
            return false;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        data = buffer.str();

        file.close();
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveManager::serializePlayerData(const Entity* player) const {
    if (!player) return "";

    std::ostringstream stream;
    stream << player->getName() << "|";
    stream << player->getHealth() << "|";
    stream << player->getMaxHealth() << "|";
    // Здесь могут быть другие данные игрока

    return stream.str();
}

bool SaveManager::deserializePlayerData(Entity* player, const std::string& data) const {
    if (!player || data.empty()) return false;

    try {
        std::istringstream stream(data);
        std::string name, healthStr, maxHealthStr;

        if (std::getline(stream, name, '|') &&
            std::getline(stream, healthStr, '|') &&
            std::getline(stream, maxHealthStr, '|')) {

            // Устанавливаем данные игрока
            // player->setName(name); // Предполагаем, что есть такой метод
            // player->setHealth(std::stoi(healthStr));
            // player->setMaxHealth(std::stoi(maxHealthStr));

            return true;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при десериализации данных игрока: " << e.what() << std::endl;
    }

    return false;
}

std::string SaveManager::serializeWorldData(const GameWorld* world) const {
    if (!world) return "";

    std::ostringstream stream;
    stream << world->getWorldName() << "|";
    // Здесь могут быть другие данные мира

    return stream.str();
}

bool SaveManager::deserializeWorldData(GameWorld* world, const std::string& data) const {
    if (!world || data.empty()) return false;

    try {
        std::istringstream stream(data);
        std::string worldName;

        if (std::getline(stream, worldName, '|')) {
            world->setWorldName(worldName);
            return true;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при десериализации данных мира: " << e.what() << std::endl;
    }

    return false;
}

std::string SaveManager::getFullSavePath(const std::string& filename) const {
    std::string cleanFilename = filename;

    // Убираем недопустимые символы
    cleanFilename.erase(std::remove_if(cleanFilename.begin(), cleanFilename.end(),
        [](char c) { return c == '/' || c == '\\' || c == ':'; }), cleanFilename.end());

    return saveDirectory + cleanFilename + ".save";
}

bool SaveManager::createSaveDirectory() const {
    try {
        std::filesystem::create_directories(saveDirectory);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании директории сохранений: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::isValidSaveFile(const std::string& filename) const {
    std::string data;
    if (!loadFromFile(filename, data)) {
        return false;
    }
    return data.find("SAVE_VERSION:" + std::to_string(saveVersion)) != std::string::npos;
}