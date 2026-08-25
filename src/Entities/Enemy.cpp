// src/Entities/Enemy.cpp
#include "Entities/Enemy.h"
#include "World/TileBasedLocation.h"
#include "Entities/Player.h"
#include "AI/AdvancedAI.h" // Нужно для создания ИИ
#include "AI/AIPersonality.h" // Для настройки личности
#include <iostream>
#include <random>
#include <cmath>
#include <sstream>
#include <memory>

Enemy::Enemy(const std::string& name, const std::string& enemyType, int health, int attack)
    : Entity(name, health), enemyType(enemyType), attackPower(attack), lootExperience(20) {
    // Создаем ИИ для врага
    AIPersonality personality;
    personality.aggression = 0.9f;
    personality.caution = 0.2f;
    personality.curiosity = 0.1f;
    personality.sociability = 0.0f;
    setAI(std::make_unique<AdvancedAI>(personality));
}

std::string Enemy::getType() const {
    return "enemy";
}

int Enemy::getAttackPower() const {
    return attackPower;
}

const std::string& Enemy::getEnemyType() const {
    return enemyType;
}

int Enemy::getLootExperience() const {
    return lootExperience;
}

// --- Сеттеры ---
void Enemy::setAttackPower(int power) {
    attackPower = power;
}

void Enemy::setEnemyType(const std::string& type) {
    enemyType = type;
}

void Enemy::setLootExperience(int exp) {
    lootExperience = exp;
}

void Enemy::takeDamage(int damage) {
    Entity::takeDamage(damage);

    // Используем ostringstream для форматирования — совместимо с C++17
    std::ostringstream oss;
    oss << getName() << " получает " << damage << " урона! Осталось здоровья: " << getHealth() << "/" << getMaxHealth() << "\n";
    std::cout << oss.str();

    if (!isAlive()) {
        // Заменяем std::format на ostringstream
        oss.str(""); // Очистка потока
        oss.clear(); // Сброс состояния ошибок
        oss << getName() << " погибает!\n";
        std::cout << oss.str();
        // Логика смерти, например, вызов onDeath
        // onDeath();
    }
}