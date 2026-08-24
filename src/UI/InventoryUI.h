// src/UI/InventoryUI.h
#ifndef INVENTORYUI_H
#define INVENTORYUI_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declarations
class Entity; // Для получения инвентаря
class Item;  // Для отображения предметов

class InventoryUI {
private:
    bool visible;
    sf::Font font;
    // --- Элементы UI ---
    sf::RectangleShape background;
    std::vector<sf::Text> itemTexts; // Тексты для отображения предметов
    sf::Text titleText; // Заголовок "Инвентарь"
    // --- Внутренние методы ---
    void updateUI(const Entity* player); // Обновляет тексты на основе инвентаря игрока
public:
    explicit InventoryUI(const sf::Font& font); // Принимаем шрифт
    ~InventoryUI() = default;
    // --- Управление видимостью ---
    void setVisible(bool isVisible);
    bool isVisible() const;
    // --- Методы цикла ---
    void update(float deltaTime, const Entity* player); // Передаем игрока для обновления
    void draw(sf::RenderTarget& target); // Исправлено: принимает sf::RenderTarget&, а не sf::RenderWindow&
    void handleEvent(const sf::Event& event);
    // Запрет копирования
    InventoryUI(const InventoryUI&) = delete;
    InventoryUI& operator=(const InventoryUI&) = delete;
};

#endif // INVENTORYUI_H