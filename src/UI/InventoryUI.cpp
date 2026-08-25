// src/UI/InventoryUI.cpp
#include "Entities/Entity.h"
#include "Items/Item.h"
#include "UI/InventoryUI.h"
#include <iostream>

InventoryUI::InventoryUI(const sf::Font& font)
    : visible(false), font(font) {
    // Настройка визуальных элементов
    background.setSize(sf::Vector2f(300, 400)); // Размер окна инвентаря
    background.setPosition(362, 100); // Примерная позиция по центру экрана (1024x768)
    background.setFillColor(sf::Color(50, 50, 50, 220)); // Темно-серый полупрозрачный
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);

    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Инвентарь");
    // Центрируем заголовок относительно фона
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(background.getPosition().x + (background.getSize().x - titleBounds.width) / 2.0f,
                          background.getPosition().y + 10);
}

void InventoryUI::setVisible(bool isVisible) {
    visible = isVisible;
}

bool InventoryUI::isVisible() const {
    return visible;
}

void InventoryUI::update(float deltaTime, const Entity* player) {
    if (!visible || !player) return;
    updateUI(player); // Обновляем содержимое при каждом вызове, если видим
}

void InventoryUI::updateUI(const Entity* player) {
    itemTexts.clear(); // Очищаем старые записи

    if (!player) return;

    const auto& inventory = player->getInventory();
    float baseX = background.getPosition().x + 10;
    float baseY = background.getPosition().y + 50; // Ниже заголовка

    for (size_t i = 0; i < inventory.size(); ++i) {
        sf::Text itemText;
        itemText.setFont(font);
        itemText.setCharacterSize(16);
        // Предполагается, что у Item есть метод getName()
        itemText.setString(inventory[i] ? inventory[i]->getName() : "Неизвестный предмет");
        itemText.setFillColor(sf::Color::White);
        itemText.setPosition(baseX, baseY + i * 25); // Расстояние 25 пикселей между строками
        itemTexts.push_back(std::move(itemText));
    }
}

void InventoryUI::draw(sf::RenderTarget& target) {
    if (!visible) return;
    target.draw(background);
    target.draw(titleText);
    for (const auto& text : itemTexts) {
        target.draw(text);
    }
}

void InventoryUI::handleEvent(const sf::Event& event) {
    if (!visible) return;
    // Пока обработка событий пуста, можно добавить, например, выбор предмета мышью
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I || event.key.code == sf::Keyboard::Escape) {
            setVisible(false); // Закрыть по I или Escape
        }
    }
}

