// src/UI/DialogSystem.cpp
#include "UI/DialogSystem.h"
#include "Entities/Player.h"
#include "Entities/Entity.h"
#include <iostream>
#include <fstream>
#include <filesystem>

DialogSystem::DialogSystem(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font), active(false) {
}

bool DialogSystem::initialize() {
    // Инициализация визуальных элементов
    dialogBox.setSize(sf::Vector2f(800, 200));
    dialogBox.setFillColor(sf::Color(0, 0, 0, 200));
    dialogBox.setOutlineThickness(2);
    dialogBox.setOutlineColor(sf::Color::White);
    dialogBox.setPosition(112, 500);

    dialogText.setFont(font);
    dialogText.setCharacterSize(18);
    dialogText.setFillColor(sf::Color::White);
    dialogText.setPosition(130, 520);

    speakerText.setFont(font);
    speakerText.setCharacterSize(20);
    speakerText.setFillColor(sf::Color::Yellow);
    speakerText.setPosition(130, 490);

    // Инициализируем пустой спрайт портрета
    portraitSprite.setPosition(50, 480);
    portraitSprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // Размер по умолчанию

    // Загрузка тестовых диалогов (можно вынести в отдельный метод)
    DialogueNode node1{ "greeting", "Привет, путник! Что привело тебя сюда?", "Старик", "", {} };
    node1.options.push_back({ "Я просто проходил мимо.", "farewell" });
    node1.options.push_back({ "Мне нужна помощь.", "quest_offer" });
    dialogTree.push_back(std::move(node1));

    DialogueNode node2{ "farewell", "Тогда будь осторожен в дороге.", "Старик", "", {} };
    node2.options.push_back({ "Спасибо, до свидания.", "" });
    dialogTree.push_back(std::move(node2));

    DialogueNode node3{ "quest_offer", "У меня как раз есть одно задание для храбреца...", "Старик", "", {} };
    node3.options.push_back({ "Расскажи подробнее.", "" });
    node3.options.push_back({ "Не сейчас.", "greeting" });
    dialogTree.push_back(std::move(node3));

    return true;
}

// ДОБАВЛЕНО: Реализация findDialogue
const DialogueNode* DialogSystem::findDialogue(const std::string& id) const {
    for (const auto& node : dialogTree) {
        if (node.id == id) {
            return &node;
        }
    }
    return nullptr;
}

// ДОБАВЛЕНО: Реализация getCurrentDialogue
const DialogueNode* DialogSystem::getCurrentDialogue() const {
    if (active && currentNodeIndex < dialogTree.size()) {
        return &dialogTree[currentNodeIndex];
    }
    return nullptr;
}
// ИСПРАВЛЕНО: startDialogue возвращает bool
bool DialogSystem::startDialogue(const std::string& dialogueId, Player* player, Entity* npc) {
    if (active || !player || !npc) {
        std::cerr << "Ошибка: невозможно начать диалог!" << std::endl;
        return false;
    }

    const DialogueNode* startNode = findDialogue(dialogueId);
    if (!startNode) {
        std::cerr << "Ошибка: диалог с ID '" << dialogueId << "' не найден!" << std::endl;
        return false;
    }

    this->player = player;
    this->npc = npc;
    active = true;
    currentNodeIndex = 0;
    loadPortraitForNode();
    updateOptionsDisplay();
    std::cout << "=== Начало диалога ===" << std::endl;
    return true;
}
void DialogSystem::endDialogue() {
    if (active) {
        std::cout << "=== Диалог завершён ===" << std::endl;
        active = false;
        player = nullptr;
        npc = nullptr;
    }
}
void DialogSystem::loadPortraitForNode() {
    if (currentNodeIndex >= dialogTree.size()) return;

    const auto& node = dialogTree[currentNodeIndex];
    if (!node.portraitPath.empty()) {
        if (sf::Texture tempTexture; tempTexture.loadFromFile(node.portraitPath)) {
            speakerPortrait = std::move(tempTexture);
            portraitSprite.setTexture(speakerPortrait);
        }
        else {
            // Загружаем запасной портрет (например, серый квадрат)
            sf::Image placeholder;
            placeholder.create(64, 64, sf::Color::Magenta);
            speakerPortrait.loadFromImage(placeholder);
            portraitSprite.setTexture(speakerPortrait);
        }
    }
    else {
        // Если портрет не указан, используем пустой
        sf::Image placeholder;
        placeholder.create(64, 64, sf::Color::Black);
        speakerPortrait.loadFromImage(placeholder);
        portraitSprite.setTexture(speakerPortrait);
    }
}

void DialogSystem::updateOptionsDisplay() {
    if (!active || currentNodeIndex >= dialogTree.size()) return;

    const auto& node = dialogTree[currentNodeIndex];

    // Обновляем текст диалога и говорящего
    dialogText.setString(node.text);
    speakerText.setString(node.speakerName + ":");

    // Очищаем старые опции
    // В этом простом случае мы не храним их отдельно, т.к. они рисуются напрямую из dialogTree

    // Не нужно создавать новые RectangleShape и Text каждый раз, если размеры не меняются.
    // Мы будем рисовать прямо из dialogTree в методе draw().
}

void DialogSystem::update(float deltaTime) {
    if (!active) return;
    // Здесь можно добавить анимацию текста, таймеры и т.п.
    // Для простоты пока ничего не делаем
}

void DialogSystem::draw(sf::RenderTarget& target) {
    if (!active) return;
    // Рисуем фон диалога
    target.draw(dialogBox);
    // Рисуем портрет говорящего
    target.draw(portraitSprite);
    // Рисуем имя говорящего
    target.draw(speakerText);
    // Рисуем текст диалога
    target.draw(dialogText);
    // Рисуем опции
    if (currentNodeIndex < dialogTree.size()) {
        const auto& node = dialogTree[currentNodeIndex];
        float startY = dialogBox.getPosition().y + dialogBox.getSize().y - 60;
        for (size_t i = 0; i < node.options.size(); ++i) {
            const auto& option = node.options[i];
            if (!option.available) continue; // Пропускаем недоступные
            // Создаем временный текст для каждой опции
            sf::Text optionText;
            optionText.setFont(font);
            optionText.setString(std::to_string(i + 1) + ". " + option.text);
            optionText.setCharacterSize(16);
            optionText.setFillColor(option.textColor);
            optionText.setPosition(130, startY + i * 35);
            target.draw(optionText);
        }
    }
}

void DialogSystem::handleEvent(const sf::Event& event) {
    if (!active) return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            // Выбор предыдущей опции (если доступна)
            if (currentNodeIndex < dialogTree.size()) {
                const auto& node = dialogTree[currentNodeIndex];
                // Можно реализовать выбор опции по индексу, но пока просто игнорируем
                // Для простоты: только Enter для подтверждения
            }
        }
        else if (event.key.code == sf::Keyboard::Down) {
            // Выбор следующей опции
            if (currentNodeIndex < dialogTree.size()) {
                const auto& node = dialogTree[currentNodeIndex];
                // Аналогично выше
            }
        }
        else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
            confirmSelection();
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            endDialogue();
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Клик мышью
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

        if (currentNodeIndex < dialogTree.size()) {
            const auto& node = dialogTree[currentNodeIndex];
            float startY = dialogBox.getPosition().y + dialogBox.getSize().y - 60;
            float startX = 130;
            float width = 780;
            float height = 30;

            for (size_t i = 0; i < node.options.size(); ++i) {
                const auto& option = node.options[i];
                if (!option.available) continue;

                sf::FloatRect rect(startX, startY + i * 35, width, height);
                if (rect.contains(mousePosF)) {
                    // Сохраняем выбранный индекс (в будущем для обработки)
                    // Но в текущей реализации сразу подтверждаем
                    confirmSelection();
                    break;
                }
            }
        }
    }
}

void DialogSystem::confirmSelection() {
    if (!active || currentNodeIndex >= dialogTree.size()) return;

    const auto& node = dialogTree[currentNodeIndex];
    if (node.options.empty()) {
        endDialogue();
        return;
    }

    // По умолчанию выбираем первую доступную опцию
    size_t selectedOptionIndex = 0;
    for (size_t i = 0; i < node.options.size(); ++i) {
        if (node.options[i].available) {
            selectedOptionIndex = i;
            break;
        }
    }

    const auto& selectedOption = node.options[selectedOptionIndex];
    std::cout << "Выбрано: \"" << selectedOption.text << "\"" << std::endl;

    // Переход к следующему узлу
    if (!selectedOption.nextDialogueId.empty()) {
        const DialogueNode* nextNode = findDialogue(selectedOption.nextDialogueId);
        if (nextNode) {
            currentNodeIndex = 0;
            // Найдем индекс следующего узла
            for (size_t i = 0; i < dialogTree.size(); ++i) {
                if (dialogTree[i].id == selectedOption.nextDialogueId) {
                    currentNodeIndex = i;
                    break;
                }
            }
            loadPortraitForNode();
            updateOptionsDisplay();
            return;
        }
    }

    // Если нет следующего ID или он пустой — заканчиваем диалог
    endDialogue();
}

bool DialogSystem::isInDialogue() const {
    return active;
}