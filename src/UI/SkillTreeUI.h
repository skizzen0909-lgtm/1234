// src/UI/SkillTreeUI.h
#ifndef SKILLTREEUI_H
#define SKILLTREEUI_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <utility>
// Forward declarations
class Entity; // Для получения навыков/уровня
class Skill;  // Предполагаемая структура навыка
struct SkillNode {
    std::string id;
    std::string name;
    std::string description;
    int levelRequired;
    bool isUnlocked;
    sf::Vector2f position; // Позиция на экране
    std::vector<std::string> dependencies; // ID навыков, необходимых для разблокировки
};
class SkillTreeUI {
private:
    sf::RenderWindow* window;
    bool visible;
    sf::Font font;
    // --- Элементы UI ---
    sf::RectangleShape background;
    std::vector<SkillNode> skillNodes; // Данные навыков
    std::vector<sf::CircleShape> nodeShapes; // Визуальные представления навыков
    std::vector<sf::Text> nodeTexts; // Тексты для названий навыков
    sf::Text titleText;
    sf::Text infoText; // Для отображения информации о выбранном навыке
    int selectedNodeIndex; // Индекс выбранного навыка (-1 если ни один)
    // --- Внутренние методы ---
    void loadSkillData(); // Загрузка/инициализация данных навыков (временно внутри)
    void updateUIPositions(); // Обновление позиций визуальных элементов
    void updateInfoText(); // Обновление текста информации о навыке
public:
    explicit SkillTreeUI(const sf::Font& font, sf::RenderWindow* window);
    ~SkillTreeUI() = default;
    // --- Управление видимостью ---
    void setVisible(bool isVisible);
    bool isVisible() const;
    // --- Методы цикла ---
    void update(float deltaTime, const Entity* player);
    void draw(sf::RenderTarget& target); // Исправлено: принимает sf::RenderTarget&, а не sf::RenderWindow&
    void handleEvent(const sf::Event& event);
    // Запрет копирования
    SkillTreeUI(const SkillTreeUI&) = delete;
    SkillTreeUI& operator=(const SkillTreeUI&) = delete;
};
#endif // SKILLTREEUI_H