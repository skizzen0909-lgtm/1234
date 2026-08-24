// src/UI/UISystem.cpp
#include "UISystem.h"
#include "Core/Engine.h"
#include "Graphics/AdvancedGraphicsSystem.h"
#include "UI/InventoryUI.h"
#include "UI/QuestLogUI.h"
#include "UI/SkillTreeUI.h"
#include "UI/DialogSystem.h"
#include "Entities/Player.h"
#include "Entities/Entity.h"
#include <iostream>
#include <sstream> 
#include <iomanip>  
#include <memory>
UISystem::UISystem()
    : initialized(false), window(nullptr), playerEntity(nullptr) {
}

UISystem::~UISystem() {
    shutdown();
}

bool UISystem::initialize(sf::RenderWindow& targetWindow) {
    if (initialized) {
        std::cerr << "UISystem уже инициализирована!" << std::endl;
        return true;
    }
    window = &targetWindow; // Используем переданное окно
    loadFont();
    setupHUD();
    // Инициализация UI панелей
    inventoryUI = std::make_unique<InventoryUI>(font);
    questLogUI = std::make_unique<QuestLogUI>(font);
    // ИСПРАВЛЕНО: передаем оба аргумента - font и window
    skillTreeUI = std::make_unique<SkillTreeUI>(font, window);
    // Получаем окно через Engine
    auto* engine = Engine::getInstance();
    if (!engine || !engine->getGraphicsSystem() || !engine->getGraphicsSystem()->isWindowOpen()) {
        std::cerr << "Ошибка: Engine или GraphicsSystem не инициализированы!" << std::endl;
        shutdown();
        return false;
    }
    // Передаем window, которое уже является graphicsWindow
    dialogSystem = std::make_unique<DialogSystem>(*window, font);
    initialized = true;
    std::cout << "UISystem initialized successfully." << std::endl;
    return true;
}

void UISystem::shutdown() {
    if (initialized) {
        messages.clear();
        inventoryUI.reset();
        questLogUI.reset();
        skillTreeUI.reset();
        dialogSystem.reset();
        window = nullptr;
        initialized = false;
        std::cout << "UISystem выключена." << std::endl;
    }
}

void UISystem::loadFont() {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Предупреждение: Не удалось загрузить шрифт 'assets/fonts/arial.ttf'. Попробуем системный..." << std::endl;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Ошибка: Не удалось загрузить ни один шрифт. UI может отображаться некорректно." << std::endl;
        }
    }
}

void UISystem::setupHUD() {
    hudText.setFont(font);
    hudText.setCharacterSize(16);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10, 10);

    hudBackground.setSize(sf::Vector2f(300, 100));
    hudBackground.setPosition(5, 5);
    hudBackground.setFillColor(sf::Color(0, 0, 0, 150));
    hudBackground.setOutlineThickness(1);
    hudBackground.setOutlineColor(sf::Color::White);
}

void UISystem::updateHUD() {
    if (!playerEntity) {
        hudText.setString("Игрок: Не найден");
        return;
    }
    std::ostringstream oss; // Теперь std::ostringstream доступен
    oss << "Игрок: " << playerEntity->getName() << "\n"
        << "Здоровье: " << playerEntity->getHealth() << "/" << playerEntity->getMaxHealth() << "\n"
        << "Уровень: " << playerEntity->getLevel() << " Опыт: " << playerEntity->getExperience();
    hudText.setString(oss.str()); // Теперь оператор << работает
}

void UISystem::showGameHUD() {
    if (!initialized) {
        std::cerr << "UISystem не инициализирована!" << std::endl;
        return;
    }
    // Отображение фона HUD
    window->draw(hudBackground);
    // Обновление и отображение текста статистики
    updateHUD();
    window->draw(hudText);
}

void UISystem::hideGameHUD() {
    hideInventory();
    hideQuestLog();
    endDialog();
}

bool UISystem::isGameHUDVisible() const {
    return initialized;
}

void UISystem::showMessage(const std::string& message) {
    if (!initialized || !window) return;
    TimedMessage msg;
    msg.text.setFont(font);
    msg.text.setCharacterSize(14);
    msg.text.setFillColor(sf::Color::Yellow);
    msg.text.setString(message);

    float baseY = window->getSize().y - 50.0f;
    float offsetY = messages.size() * 20.0f;
    msg.text.setPosition(10, baseY - offsetY);
    msg.timer.restart();
    messages.push_back(std::move(msg));

    if (messages.size() > 5) {
        messages.erase(messages.begin());
    }
}

void UISystem::clearMessages() {
    messages.clear();
}

void UISystem::showDialog(const std::string& dialogueId, Player* player, Entity* npc) {
    if (dialogSystem) {
        dialogSystem->startDialogue(dialogueId, player, npc);
    }
}

void UISystem::endDialog() {
    if (dialogSystem) {
        dialogSystem->endDialogue();
    }
}

bool UISystem::isInDialog() const {
    return dialogSystem && dialogSystem->isInDialogue();
}

DialogSystem* UISystem::getDialogSystem() const {
    return dialogSystem.get();
}

void UISystem::setPlayerEntity(Entity* player) {
    playerEntity = player;
}

void UISystem::showInventory() {
    if (inventoryUI) inventoryUI->setVisible(true);
}

void UISystem::hideInventory() {
    if (inventoryUI) inventoryUI->setVisible(false);
}

bool UISystem::isInventoryVisible() const {
    return inventoryUI && inventoryUI->isVisible();
}

void UISystem::showQuestLog() {
    if (questLogUI) questLogUI->setVisible(true);
}

void UISystem::hideQuestLog() {
    if (questLogUI) questLogUI->setVisible(false);
}

bool UISystem::isQuestLogVisible() const {
    return questLogUI && questLogUI->isVisible();
}

void UISystem::showSkillTree() {
    if (skillTreeUI) skillTreeUI->setVisible(true);
}

void UISystem::hideSkillTree() {
    if (skillTreeUI) skillTreeUI->setVisible(false);
}

bool UISystem::isSkillTreeVisible() const {
    return skillTreeUI && skillTreeUI->isVisible();
}

void UISystem::update(float deltaTime) {
    if (!initialized || !window) return;

    messages.erase(
        std::remove_if(messages.begin(), messages.end(),
            [](const TimedMessage& msg) { return msg.timer.getElapsedTime().asSeconds() > 3.0f; }),
        messages.end());

    updateHUD();

    if (inventoryUI && inventoryUI->isVisible()) {
        inventoryUI->update(deltaTime, playerEntity);
    }
    if (questLogUI && questLogUI->isVisible()) {
        questLogUI->update(deltaTime, playerEntity);
    }
    if (skillTreeUI && skillTreeUI->isVisible()) {
        skillTreeUI->update(deltaTime, playerEntity);
    }
    if (dialogSystem && dialogSystem->isInDialogue()) {
        dialogSystem->update(deltaTime);
    }
}

void UISystem::draw(sf::RenderTarget& target) {
    if (!isActive()) return;
    // Рисуем HUD и сообщения
    target.draw(hudBackground);
    target.draw(hudText);
    for (auto& msg : messages) {
        target.draw(msg.text);
    }
    // Рисуем инвентарь, журнал квестов и дерево навыков
    if (inventoryUI && inventoryUI->isVisible()) {
        inventoryUI->draw(target);
    }
    if (questLogUI && questLogUI->isVisible()) {
        questLogUI->draw(target);
    }
    if (skillTreeUI && skillTreeUI->isVisible()) {
        skillTreeUI->draw(target);
    }
    // Наконец, рисуем диалоги поверх всех остальных элементов
    if (dialogSystem && dialogSystem->isInDialogue()) {
        dialogSystem->draw(target);
    }
}

bool UISystem::isActive() const {
    return (inventoryUI && inventoryUI->isVisible()) ||
        (questLogUI && questLogUI->isVisible()) ||
        (skillTreeUI && skillTreeUI->isVisible()) ||
        (dialogSystem && dialogSystem->isInDialogue());
}

void UISystem::handleEvent(const sf::Event& event) {
    if (!initialized) return;

    if (dialogSystem && dialogSystem->isInDialogue()) {
        dialogSystem->handleEvent(event);
        return;
    }

    if (inventoryUI && inventoryUI->isVisible()) {
        inventoryUI->handleEvent(event);
        if (!inventoryUI->isVisible()) return;
    }
    if (questLogUI && questLogUI->isVisible()) {
        questLogUI->handleEvent(event);
        if (!questLogUI->isVisible()) return;
    }
    if (skillTreeUI && skillTreeUI->isVisible()) {
        skillTreeUI->handleEvent(event);
    }
}

void UISystem::hideAll() {
    hideGameHUD();
    hideInventory();
    hideQuestLog();
    hideSkillTree();
}