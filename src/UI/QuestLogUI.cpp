// src/UI/QuestLogUI.cpp
#include "UI/QuestLogUI.h"
#include "Entities/Entity.h"
#include "Quests/Quest.h" // Предполагается, что у Quest есть getTitle(), getDescription(), isCompleted()
#include <iostream>

QuestLogUI::QuestLogUI(const sf::Font& font)
    : visible(false), font(font) {
    // Настройка визуальных элементов
    background.setSize(sf::Vector2f(400, 300)); // Размер окна журнала
    background.setPosition(312, 200); // Примерная позиция
    background.setFillColor(sf::Color(40, 40, 80, 220)); // Темно-синий полупрозрачный
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::Cyan);

    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setString("Журнал квестов");
    // Центрируем заголовок
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(background.getPosition().x + (background.getSize().x - titleBounds.width) / 2.0f,
                          background.getPosition().y + 10);
}

void QuestLogUI::setVisible(bool isVisible) {
    visible = isVisible;
}

bool QuestLogUI::isVisible() const {
    return visible;
}

void QuestLogUI::update(float deltaTime, const Entity* player) {
    if (!visible || !player) return;
    updateUI(player); // Обновляем содержимое
}

void QuestLogUI::updateUI(const Entity* player) {
    questTexts.clear(); // Очищаем старые записи

    if (!player) return;

    const auto& quests = player->getActiveQuests(); // Предполагается, что есть такой метод
    float baseX = background.getPosition().x + 10;
    float baseY = background.getPosition().y + 50;

    for (size_t i = 0; i < quests.size(); ++i) {
        sf::Text questText;
        questText.setFont(font);
        questText.setCharacterSize(16);
        if (quests[i]) {
            std::string questStatus = quests[i]->isCompleted() ? "[Завершен] " : "[Активен] ";
            questText.setString(questStatus + quests[i]->getTitle());
            questText.setFillColor(quests[i]->isCompleted() ? sf::Color::Green : sf::Color::Yellow);
            // Можно добавить описание на следующей строке, если нужно
            // sf::Text descText;
            // descText.setFont(font);
            // descText.setCharacterSize(14);
            // descText.setString(quests[i]->getDescription());
            // descText.setFillColor(sf::Color::White);
            // descText.setPosition(baseX + 20, baseY + i * 40 + 20); // Слегка смещенное и меньше
        } else {
            questText.setString("Неизвестный квест");
            questText.setFillColor(sf::Color::Red);
        }
        questText.setPosition(baseX, baseY + i * 30); // Больше расстояние для потенциального описания
        questTexts.push_back(std::move(questText));
    }
}

void QuestLogUI::draw(sf::RenderTarget& target) {
    if (!visible) return;
    target.draw(background);
    target.draw(titleText);
    for (const auto& text : questTexts) {
        target.draw(text);
    }
}

void QuestLogUI::handleEvent(const sf::Event& event) {
    if (!visible) return;
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::L || event.key.code == sf::Keyboard::Escape) {
            setVisible(false); // Закрыть по L или Escape
        }
    }
}