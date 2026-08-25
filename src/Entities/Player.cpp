// Entities/Player.cpp
#include "Player.h"
#include "World/TileBasedLocation.h"
#include "World/TileMap.h"
#include "AI/AdvancedAI.h"
#include "AI/AIPersonality.h"
#include "NPC.h"  
#include <iostream>
#include <random>
#include <sstream>

Player::Player(const std::string& name, int health)
    : Entity(name, health)
    , level(1)
    , experience(0)
    , experienceToNextLevel(100)
    , gen(std::random_device{}())
    , moveDist(-1, 1)
{
    // ИИ создаётся в EnhancedPlayState::initializeAI()
}

std::string Player::getType() const {
    return "player";
}

void Player::takeDamage(int damage) {
    Entity::takeDamage(damage);
    std::ostringstream oss;
    oss << getName() << " получает " << damage << " урона!";
    std::cout << oss.str() << std::endl;
}

void Player::heal(int amount) {
    Entity::heal(amount);
    std::ostringstream oss;
    oss << getName() << " восстанавливает " << amount << " здоровья!";
    std::cout << oss.str() << std::endl;
}

void Player::performAutoAction(float deltaTime) {
    if (!currentLocation) return;

    auto [posX, posY] = getPosition();
    if (posX == -1 || posY == -1) return;

    TileBasedLocation* tileLoc = dynamic_cast<TileBasedLocation*>(currentLocation);
    if (!tileLoc || !tileLoc->getTileMap()) return;

    // === 1. АВТОМАТИЧЕСКОЕ ВЗАИМОДЕЙСТВИЕ С NPC ===
    const auto& entitiesOnTile = tileLoc->getTileMap()->getEntities(posX, posY);
    for (const auto& entity : entitiesOnTile) {
        if (!entity || entity.get() == this) continue;

        // Проверяем, является ли сущность NPC
        if (entity->getType() == "npc" || entity->getType() == "villager") {
            // Приводим к NPC* безопасно
            if (auto npc = std::dynamic_pointer_cast<NPC>(entity)) {
                // Проверяем, не в диалоге ли уже (через UI — опционально, пока пропустим)
                std::cout << getName() << " автоматически разговаривает с " << npc->getName() << ".\n";
                npc->interact(this); // <-- Правильно: NPC взаимодействует с игроком
                return; // Взаимодействуем только с одним NPC за кадр
            }
        }
    }

    // === 2. ЗАГЛУШКА ДЛЯ СБОРА ЛУТА ===
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> chance(0.0, 1.0);

    if (chance(gen) < 0.005f) { // 0.5% шанс за кадр (~раз в 3 сек при 60 FPS)
        std::cout << getName() << " находит что-то интересное на земле!\n";
        // TODO: В будущем — вызов системы лута
    }
}

int Player::getLevel() const {
    return level;
}

int Player::getExperience() const {
    return experience;
}

int Player::getExperienceToNextLevel() const {
    return experienceToNextLevel;
}

void Player::setLevel(int lvl) {
    if (lvl > 0) {
        level = lvl;
    }
}

void Player::setExperience(int exp) {
    if (exp >= 0) {
        experience = exp;
    }
}

void Player::setExperienceToNextLevel(int exp) {
    if (exp > 0) {
        experienceToNextLevel = exp;
    }
}

void Player::gainExperience(int exp) {
    if (exp <= 0) return;
    experience += exp;
    std::ostringstream oss;
    oss << getName() << " получает " << exp << " опыта! Всего: " << experience << "";
    std::cout << oss.str();
    levelUp();
}

void Player::levelUp() {
    while (experience >= experienceToNextLevel) {
        level++;
        experience -= experienceToNextLevel;
        experienceToNextLevel = level * 100;
        std::ostringstream oss;
        oss << "Поздравляем! " << getName() << " достиг " << level << " уровня!" << "";
        std::cout << oss.str();
    }
}