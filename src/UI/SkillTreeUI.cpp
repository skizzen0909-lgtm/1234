// src/UI/SkillTreeUI.cpp
#include "UI/SkillTreeUI.h"
#include "Entities/Entity.h"
#include <iostream>
#include <sstream>
#include <cmath>

SkillTreeUI::SkillTreeUI(const sf::Font& font, sf::RenderWindow* window)
    : visible(false), font(font), selectedNodeIndex(-1), window(window) {
    // Настройка визуальных элементов
    background.setSize(sf::Vector2f(500, 400));
    background.setPosition(262, 150); // Центрируем для 1024x768
    background.setFillColor(sf::Color(30, 60, 30, 220)); // Темно-зеленый
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::Green);

    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::Green);
    titleText.setString("Дерево Навыков");
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(background.getPosition().x + (background.getSize().x - titleBounds.width) / 2.0f,
        background.getPosition().y + 10);

    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(background.getPosition().x + 10, background.getPosition().y + background.getSize().y - 80);

    loadSkillData(); // Загружаем тестовые данные
    updateUIPositions(); // Инициализируем визуальные элементы
}

void SkillTreeUI::loadSkillData() {
    // Тестовые данные навыков
    skillNodes.clear();
    SkillNode node1{ "str1", "Сила+", "Увеличивает урон на 5%", 1, true, sf::Vector2f(250, 100), {} };
    skillNodes.push_back(node1);
    SkillNode node2{ "dex1", "Ловкость+", "Увеличивает шанс уклонения на 5%", 1, true, sf::Vector2f(150, 180), {} };
    skillNodes.push_back(node2);
    SkillNode node3{ "int1", "Интеллект+", "Увеличивает ману на 10", 1, true, sf::Vector2f(350, 180), {} };
    skillNodes.push_back(node3);
    SkillNode node4{ "str2", "Геркулес", "Увеличивает урон на 10%", 3, false, sf::Vector2f(250, 260), {"str1"} };
    skillNodes.push_back(node4);

    // Инициализируем визуальные элементы для узлов
    nodeShapes.clear();
    nodeTexts.clear();
    for (size_t i = 0; i < skillNodes.size(); ++i) {
        sf::CircleShape shape(20); // Радиус 20
        shape.setOrigin(20, 20); // Центрируем
        shape.setPosition(skillNodes[i].position.x + background.getPosition().x,
            skillNodes[i].position.y + background.getPosition().y);
        shape.setFillColor(skillNodes[i].isUnlocked ? sf::Color::Yellow : sf::Color::Blue);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
        nodeShapes.push_back(shape);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::Black);
        text.setString(skillNodes[i].name.substr(0, 3)); // Аббревиатура
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(shape.getPosition().x - textBounds.width / 2.0f,
            shape.getPosition().y - textBounds.height / 2.0f - 5); // Центрируем текст
        nodeTexts.push_back(std::move(text));
    }
}

void SkillTreeUI::updateUIPositions() {
    // Если размеры фона или позиции узлов изменяются, обновить позиции shape и text
    // Пока позиции фиксированы в loadSkillData
}

void SkillTreeUI::updateInfoText() {
    if (selectedNodeIndex >= 0 && selectedNodeIndex < static_cast<int>(skillNodes.size())) {
        const auto& node = skillNodes[selectedNodeIndex];
        std::ostringstream oss;
        oss << "Навык: " << node.name << "\n";
        oss << "Описание: " << node.description << "\n";
        oss << "Требуемый уровень: " << node.levelRequired << "\n";
        oss << "Статус: " << (node.isUnlocked ? "Изучен" : "Недоступен");
        infoText.setString(oss.str());
    }
    else {
        infoText.setString("Выберите навык для получения информации.");
    }
}

void SkillTreeUI::setVisible(bool isVisible) {
    visible = isVisible;
    if (isVisible) {
        selectedNodeIndex = -1; // Сброс выбора при открытии
        updateInfoText();
    }
}

bool SkillTreeUI::isVisible() const {
    return visible;
}

void SkillTreeUI::update(float deltaTime, const Entity* player) {
    if (!visible) return;
    // Обновление состояния навыков на основе игрока (если бы это было связано)
    // Например, проверка уровня игрока и разблокировка навыков
    // for (auto& node : skillNodes) {
    //     if (!node.isUnlocked && player && player->getLevel() >= node.levelRequired) {
    //         // Проверить зависимости
    //         bool depsMet = true;
    //         for(const auto& depId : node.dependencies) {
    //             auto depIt = std::find_if(skillNodes.begin(), skillNodes.end(),
    //                 [&depId](const SkillNode& n) { return n.id == depId; });
    //             if (depIt == skillNodes.end() || !depIt->isUnlocked) {
    //                 depsMet = false;
    //                 break;
    //             }
    //         }
    //         if (depsMet) {
    //             node.isUnlocked = true;
    //             // Обновить цвет shape
    //             // nodeShapes[...].setFillColor(sf::Color::Yellow);
    //         }
    //     }
    // }
    // updateUIPositions(); // Если состояние изменилось
}

void SkillTreeUI::draw(sf::RenderTarget& target) {
    if (!visible) return;
    target.draw(background);
    target.draw(titleText);
    // Рисуем линии зависимостей (упрощенно)
    for (size_t i = 0; i < skillNodes.size(); ++i) {
        for (const auto& depId : skillNodes[i].dependencies) {
            auto depIt = std::find_if(skillNodes.begin(), skillNodes.end(),
                [&depId](const SkillNode& n) { return n.id == depId; });
            if (depIt != skillNodes.end()) {
                size_t depIndex = std::distance(skillNodes.begin(), depIt);
                if (depIndex < nodeShapes.size()) {
                    sf::Vertex line[] = {
                        sf::Vertex(nodeShapes[i].getPosition(), sf::Color::White),
                        sf::Vertex(nodeShapes[depIndex].getPosition(), sf::Color::White)
                    };
                    // Используем target вместо window
                    target.draw(line, 2, sf::Lines);
                }
            }
        }
    }
    // Рисуем узлы
    for (size_t i = 0; i < nodeShapes.size(); ++i) {
        target.draw(nodeShapes[i]);
        target.draw(nodeTexts[i]);
    }
    target.draw(infoText);
}

void SkillTreeUI::handleEvent(const sf::Event& event) {
    if (!visible) return;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        selectedNodeIndex = -1;
        for (size_t i = 0; i < nodeShapes.size(); ++i) {
            // Проверка попадания в круг (упрощенно)
            float dx = mousePosF.x - nodeShapes[i].getPosition().x;
            float dy = mousePosF.y - nodeShapes[i].getPosition().y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance <= nodeShapes[i].getRadius()) {
                selectedNodeIndex = static_cast<int>(i);
                // Выделяем выбранный узел (например, меняем цвет обводки)
                for (auto& shape : nodeShapes) {
                    shape.setOutlineColor(sf::Color::White); // Сброс всех...
                }
                nodeShapes[selectedNodeIndex].setOutlineColor(sf::Color::Red); // Выделение
                break;
            }
        }
        updateInfoText();
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::K || event.key.code == sf::Keyboard::Escape) {
            setVisible(false);
        }
    }
}