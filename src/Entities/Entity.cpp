// src/Entities/Entity.cpp
#include "Entities/Entity.h"
#include "Items/Item.h" // Добавлено: включаем Item.h
#include "Quests/Quest.h" // Добавлено: включаем Quest.h
#include "AI/AIBase.h" // Добавлено: включаем AIBase.h
#include "World/TileBasedLocation.h" 
#include "World/Location.h"
#include "Events/EventManager.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>

Entity::Entity(const std::string& name, int health)
    : name(name), health(health), maxHealth(health),
    posX(-1), posY(-1), currentLocation(nullptr),
    aiUpdateTimer(0.0f), moveTimer(0.0f),
    level(1), experience(0),
    plannedMoveX(-1), plannedMoveY(-1), hasPlannedMove(false) 
{
}

// --- Геттеры ---
const std::string& Entity::getName() const { return name; }
int Entity::getHealth() const { return health; }
int Entity::getMaxHealth() const { return maxHealth; }
std::pair<int, int> Entity::getPosition() const { return { posX, posY }; }
Location* Entity::getCurrentLocation() const { return currentLocation; }
const std::vector<std::shared_ptr<Item>>& Entity::getInventory() const { return inventory; }
const std::vector<std::shared_ptr<Quest>>& Entity::getActiveQuests() const { return activeQuests; }
int Entity::getLevel() const { return level; }
int Entity::getExperience() const { return experience; }
int Entity::getStat(const std::string& statName) const {
    auto it = stats.find(statName);
    return (it != stats.end()) ? it->second : 0;
}

// --- Сеттеры ---
void Entity::setName(const std::string& newName) { name = newName; }
void Entity::setHealth(int newHealth) {
    health = std::max(0, std::min(newHealth, maxHealth));
}
void Entity::setMaxHealth(int newMaxHealth) { maxHealth = newMaxHealth; }
void Entity::setPosition(int x, int y) {
    if (currentLocation) {
        TileBasedLocation* tileLoc = dynamic_cast<TileBasedLocation*>(currentLocation);
        if (tileLoc && tileLoc->getTileMap()) {
            if (!tileLoc->getTileMap()->isValidPosition(x, y)) {
                // Обработка ошибки: установка позиции в безопасное место
                x = std::max(0, std::min(x, tileLoc->getTileMap()->getWidth() - 1));
                y = std::max(0, std::min(y, tileLoc->getTileMap()->getHeight() - 1));
            }
        }
    }
    posX = x;
    posY = y;
}
void Entity::setCurrentLocation(Location* location) { currentLocation = location; }

// --- Логика ---
bool Entity::isAlive() const { return health > 0; }
void Entity::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        if (health < 0) health = 0;
        std::ostringstream oss;
        oss << name << " получил " << damage << " урона. Здоровье: " << health << "/" << maxHealth << "\n";
        std::cout << oss.str();
    }
}
void Entity::heal(int amount) {
    if (amount > 0) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::ostringstream oss;
        oss << name << " восстановил " << amount << " здоровья. Здоровье: " << health << "/" << maxHealth << "\n"; 
        std::cout << oss.str();
    }
}
void Entity::gainExperience(int exp) {
    if (exp > 0) {
        experience += exp;
        std::ostringstream oss;
        oss << name << " получил " << exp << " опыта. Всего: " << experience << "\n"; 
        std::cout << oss.str();
    }
}
void Entity::levelUp() {
    level++;
    experience = 0;
    maxHealth += 10;
    health = maxHealth;
    std::ostringstream oss; 
    oss << name << " достиг уровня " << level << "!\n"; 
    std::cout << oss.str();
}
void Entity::addItem(std::shared_ptr<Item> item) {
    if (item) {
        inventory.push_back(item);
        std::ostringstream oss; 
        oss << name << " получил предмет: " << item->getName() << "\n"; 
        std::cout << oss.str();
    }
}
void Entity::removeItem(std::shared_ptr<Item> item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
        std::ostringstream oss; 
        oss << name << " потерял предмет: " << item->getName() << "\n"; 
        std::cout << oss.str();
    }
}
bool Entity::hasItem(const std::string& itemName) const {
    return std::any_of(inventory.begin(), inventory.end(),
        [&itemName](const std::shared_ptr<Item>& item) {
            return item && item->getName() == itemName;
        });
}
void Entity::addQuest(std::shared_ptr<Quest> quest) {
    if (quest) {
        activeQuests.push_back(quest);
        std::ostringstream oss; 
        oss << name << " взял квест: " << quest->getTitle() << "\n"; 
        std::cout << oss.str();
    }
}
void Entity::completeQuest(std::shared_ptr<Quest> quest) {
    auto it = std::find(activeQuests.begin(), activeQuests.end(), quest);
    if (it != activeQuests.end()) {
        (*it)->setCompleted(true);
        std::ostringstream oss;
        oss << name << " завершил квест: " << quest->getTitle() << "\n";
        std::cout << oss.str();        activeQuests.erase(it);
    }
}

// --- ИИ ---
void Entity::setAI(std::unique_ptr<AIBase> newAI) {
    ai = std::move(newAI);
}

AIBase* Entity::getAI() const {
    return ai.get();
}

void Entity::updateAI(float deltaTime) {
    if (ai) {
        ai->update(deltaTime, this);
    }
}

// --- Движение ---
std::pair<int, int> Entity::decideMovement() {
    // Базовая реализация или делегирование ИИ
    return { posX, posY };
}

void Entity::updateMovement(float deltaTime) {
    moveTimer += deltaTime;
    if (moveTimer >= moveInterval) {
        if (hasPlannedMove) {
            // Добавлена явная проверка, что plannedMove не (-1, -1)
            // Это защищает от установки позиции в (-1, -1), если plannedMove был таким
            if (plannedMoveX != -1 && plannedMoveY != -1) {
                // Добавлена проверка, что текущая позиция допустима перед перемещением
                // Это помогает, если сущность уже в (-1, -1)
                if (posX != -1 && posY != -1) {
                    Location* currentLoc = getCurrentLocation();
                    TileBasedLocation* tileLoc = dynamic_cast<TileBasedLocation*>(currentLoc);

                    if (tileLoc && tileLoc->getTileMap()) {
                        // Теперь безопасно вызываем getTileMap() и проверяем новую позицию
                        if (tileLoc->getTileMap()->isValidPosition(plannedMoveX, plannedMoveY)) {
                            setPosition(plannedMoveX, plannedMoveY);
                        }
                        else {
                            // Новая позиция недопустима, не двигаемся
                            // Можно добавить логирование здесь для отладки
                            // std::cerr << "Entity::updateMovement: Invalid planned move to (" << plannedMoveX << ", " << plannedMoveY << ")" << std::endl;
                        }
                    }
                    else {
                        // std::cerr << "Entity::updateMovement: Location is not TileBased or has no tilemap." << std::endl;
                    }
                }
                else {
                    // std::cerr << "Entity::updateMovement: Entity has invalid current position (" << posX << ", " << posY << "). Cannot move from here." << std::endl;
                }
            }
            else {
                // std::cerr << "Entity::updateMovement: Planned move is (-1, -1). Not moving." << std::endl;
            }
        }
        // В любом случае сбрасываем планируемое движение
        hasPlannedMove = false;
        plannedMoveX = -1;
        plannedMoveY = -1;
        moveTimer = 0.0f;
    }
}

void Entity::update(float deltaTime) {
    // Обновление ИИ каждый кадр с реальным deltaTime
    if (ai) {
        ai->update(deltaTime, this);
    }
    // Обновление движения
    updateMovement(deltaTime);
}