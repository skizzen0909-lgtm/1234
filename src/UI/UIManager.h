// UI/UIManager.h
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Entity;
class Quest;
class Inventory;

class UIManager {
private:
    bool initialized;

public:
    // Конструктор/деструктор
    UIManager();
    ~UIManager() = default;

    // Инициализация и очистка
    bool initialize();
    void shutdown();

    // Методы отображения
    void displayPlayerStats(const Entity* player) const;
    void displayInventory(const Inventory* inventory) const;
    void displayQuests(const std::vector<std::shared_ptr<Quest>>& quests) const;
    void displayLocationInfo(const std::string& locationName, const std::string& description) const;

    // Методы ввода
    int getChoice(int min, int max) const;
    std::string getStringInput(const std::string& prompt) const;

    // Методы сообщений
    void showMessage(const std::string& message) const;
    void showError(const std::string& error) const;
    void showCombatMessage(const std::string& attacker, const std::string& defender, int damage) const;

    // Статус
    bool isInitialized() const;

    // Запрет копирования
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
};

#endif // UIMANAGER_H