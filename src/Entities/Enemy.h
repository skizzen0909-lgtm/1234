// src/Entities/Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Entities/Entity.h"
#include <string>

class Enemy : public Entity {
private:
    std::string enemyType;
    int attackPower;
    int lootExperience;

public:
    Enemy(const std::string& name, const std::string& enemyType, int health = 50, int attack = 10);
    ~Enemy() override = default;

    // Переопределение виртуальных методов
    std::string getType() const override;

    // Геттеры
    int getAttackPower() const;
    const std::string& getEnemyType() const;
    int getLootExperience() const;

    // Сеттеры (добавлены)
    void setAttackPower(int power);
    void setEnemyType(const std::string& type);
    void setLootExperience(int exp);

    // Методы
    void takeDamage(int damage) override;
    // void update(float deltaTime) override; // Убран, используем базовый или добавим позже, если нужно специфическое поведение

    // Запрет копирования (наследуется от Entity)
};

#endif // ENEMY_H