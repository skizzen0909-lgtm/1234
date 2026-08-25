// src/UI/QuestLogUI.h
#ifndef QUESTLOGUI_H
#define QUESTLOGUI_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
// Forward declarations
class Entity; // Для получения квестов
class Quest; // Для отображения квестов
class QuestLogUI {
private:
    bool visible;
    sf::Font font;
    // --- Элементы UI ---
    sf::RectangleShape background;
    std::vector<sf::Text> questTexts; // Тексты для отображения квестов
    sf::Text titleText; // Заголовок "Журнал квестов"
    // --- Внутренние методы ---
    void updateUI(const Entity* player); // Обновляет тексты на основе активных квестов игрока
public:
    explicit QuestLogUI(const sf::Font& font); // Принимаем шрифт
    ~QuestLogUI() = default;
    // --- Управление видимостью ---
    void setVisible(bool isVisible);
    bool isVisible() const;
    // --- Методы цикла ---
    void update(float deltaTime, const Entity* player); // Передаем игрока для обновления
    void draw(sf::RenderTarget& target); // Исправлено: принимает sf::RenderTarget&, а не sf::RenderWindow&
    void handleEvent(const sf::Event& event);
    // Запрет копирования
    QuestLogUI(const QuestLogUI&) = delete;
    QuestLogUI& operator=(const QuestLogUI&) = delete;
};
#endif // QUESTLOGUI_H