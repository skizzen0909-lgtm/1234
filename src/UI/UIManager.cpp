#include "UI/UIManager.h"
#include "Entities/Entity.h"
#include "Quests/Quest.h"
#include "Inventory/Inventory.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

UIManager::UIManager() : initialized(false) {
}

bool UIManager::initialize() {
    // Здесь может быть инициализация графической библиотеки
    // Пока используем консольный интерфейс
    initialized = true;
    std::cout << "UI Manager инициализирован" << std::endl;
    return true;
}

void UIManager::shutdown() {
    if (initialized) {
        std::cout << "UI Manager завершает работу" << std::endl;
        initialized = false;
    }
}

void UIManager::displayPlayerStats(const Entity* player) const {
    if (!player || !initialized) return;

    std::ostringstream oss;
    oss << "\n=== Статистика игрока ===\n"
        << "Имя: " << player->getName() << "\n"
        << "Здоровье: " << player->getHealth() << "/" << player->getMaxHealth() << "\n"
        << "========================";
    std::cout << oss.str() << std::endl;
}

void UIManager::displayInventory(const Inventory* inventory) const {
    if (!inventory || !initialized) return;

    std::ostringstream oss;
    oss << "\n=== Инвентарь ===\n"
        << "Предметов: " << inventory->getItemCount() << "/" << inventory->getMaxCapacity() << "\n";
    std::cout << oss.str() << std::endl;

    const auto& items = inventory->getItems();
    if (items.empty()) {
        std::cout << "Инвентарь пуст" << std::endl;
    }
    else {
        for (size_t i = 0; i < items.size(); ++i) {
            // Здесь должна быть логика отображения предметов
            std::cout << (i + 1) << ". [Предмет]" << std::endl;
        }
    }
    std::cout << "=================" << std::endl;
}

void UIManager::displayQuests(const std::vector<std::shared_ptr<Quest>>& quests) const {
    if (!initialized) return;

    std::cout << "\n=== Активные квесты ===" << std::endl;
    if (quests.empty()) {
        std::cout << "Нет активных квестов" << std::endl;
    }
    else {
        for (const auto& quest : quests) {
            if (quest) {
                std::cout << "- " << quest->getTitle() << std::endl;
                std::cout << "  " << quest->getDescription() << std::endl;
                std::cout << "  Прогресс: " << quest->getProgressString() << std::endl;
            }
        }
    }
    std::cout << "======================" << std::endl;
}

void UIManager::displayLocationInfo(const std::string& locationName, const std::string& description) const {
    if (!initialized) return;

    std::ostringstream oss;
    oss << "\n=== Текущая локация ===\n"
        << "Название: " << locationName << "\n"
        << "Описание: " << description << "\n"
        << "=======================";
    std::cout << oss.str() << std::endl;
}

int UIManager::getChoice(int min, int max) const {
    if (!initialized) return -1;
    int choice;
    while (true) {
        std::ostringstream oss;
        oss << "Введите выбор (" << min << "-" << max << "): ";
        std::cout << oss.str();

        if (std::cin >> choice && choice >= min && choice <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        else {
            std::cout << "Неверный ввод. Попробуйте снова." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string UIManager::getStringInput(const std::string& prompt) const {
    if (!initialized) return "";
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void UIManager::showMessage(const std::string& message) const {
    if (!initialized) return;
    std::cout << message << std::endl;
}

void UIManager::showError(const std::string& error) const {
    if (!initialized) return;
    std::cerr << "ОШИБКА: " << error << std::endl;
}

void UIManager::showCombatMessage(const std::string& attacker, const std::string& defender, int damage) const {
    if (!initialized) return;

    std::ostringstream oss;
    oss << attacker << " наносит " << damage << " урона " << defender << "!";
    std::cout << oss.str() << std::endl;
}

bool UIManager::isInitialized() const {
    return initialized;
}