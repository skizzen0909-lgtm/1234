// src/UI/UISystem.h
#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "Core/Engine.h"  
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Forward declarations
class Entity;
class Player;
class QuestLogUI;
class InventoryUI;
class SkillTreeUI;
class DialogSystem;
class AdvancedGraphicsSystem;

class UISystem {
private:
    bool initialized;
    sf::RenderWindow* window;
    Entity* playerEntity;

    // --- Основные элементы UI ---
    sf::Font font;
    sf::Text hudText;
    sf::RectangleShape hudBackground;

    // --- UI Панели ---
    std::unique_ptr<InventoryUI> inventoryUI;
    std::unique_ptr<QuestLogUI> questLogUI;
    std::unique_ptr<SkillTreeUI> skillTreeUI;
    std::unique_ptr<DialogSystem> dialogSystem;

    // --- Сообщения ---
    struct TimedMessage {
        sf::Text text;
        sf::Clock timer;
    };
    std::vector<TimedMessage> messages;

    // --- Диалоговые элементы ---
    sf::RectangleShape dialogBox;
    sf::Text speakerText;
    sf::Text dialogText;
    sf::Texture speakerPortrait;
    sf::Sprite portraitSprite;
    std::vector<sf::RectangleShape> optionBoxes;
    std::vector<sf::Text> optionTexts;

    // --- Внутренние методы ---
    void loadFont();
    void setupHUD();
    void updateHUD();

public:
    UISystem();
    ~UISystem();

    // Инициализация и очистка — ТОЛЬКО ОБЪЯВЛЕНИЕ!
    bool initialize(sf::RenderWindow& targetWindow);
    void shutdown();

    // Основные методы UI
    void showGameHUD();
    void hideGameHUD();
    bool isGameHUDVisible() const;

    // Сообщения
    void showMessage(const std::string& message);
    void clearMessages();

    void showDialog(const std::string& dialogueId, Player* player, Entity* npc);
    void endDialog();
    bool isInDialog() const;
    DialogSystem* getDialogSystem() const;

    void setPlayerEntity(Entity* player);

    // Панели
    void showInventory();
    void hideInventory();
    bool isInventoryVisible() const;

    void showQuestLog();
    void hideQuestLog();
    bool isQuestLogVisible() const;

    void showSkillTree();
    void hideSkillTree();
    bool isSkillTreeVisible() const;

    // Методы цикла
    void update(float deltaTime);
    void draw(sf::RenderTarget& target);
    bool isActive() const;
    void handleEvent(const sf::Event& event);
    void hideAll(); // Теперь публичный метод
};

#endif // UISYSTEM_H