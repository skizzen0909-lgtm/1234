/// src/Entities/Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include "AI/AIBase.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
// Forward declarations
class Location;
class AIBase;
class CombatEvent;
class Quest;
class Item;
class TileBasedLocation;
class Entity : public std::enable_shared_from_this<Entity> {
protected:
    std::string name;
    int health;
    int maxHealth;
    int posX, posY;
    Location* currentLocation;
    // --- ИИ ---
    std::unique_ptr<AIBase> ai;
    float aiUpdateTimer = 0.0f;
    // --- Движение ---
    float moveTimer = 0.0f;     
    static constexpr float moveInterval = 0.5f;
    // Сделаем поля публичными для прямого доступа ИИ (или создать сеттеры)
public: // <<< ИЗМЕНЕНО С protected НА public
    int plannedMoveX;
    int plannedMoveY;
    bool hasPlannedMove;
protected: // <<< Вернемся к protected для остальных полей
    // --- Инвентарь и квесты ---
    std::vector<std::shared_ptr<Item>> inventory;
    std::vector<std::shared_ptr<Quest>> activeQuests;
    // --- Статистика ---
    int level;
    int experience;
    std::unordered_map<std::string, int> stats;
public:
    Entity(const std::string& name, int health = 100);
    virtual ~Entity() = default;
    // Геттеры
    const std::string& getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    std::pair<int, int> getPosition() const;
    Location* getCurrentLocation() const;
    const std::vector<std::shared_ptr<Item>>& getInventory() const;
    const std::vector<std::shared_ptr<Quest>>& getActiveQuests() const;
    int getLevel() const;
    int getExperience() const;
    int getStat(const std::string& statName) const;
    // Сеттеры
    void setName(const std::string& newName);
    virtual void setHealth(int newHealth);
    void setMaxHealth(int newMaxHealth);
    void setPosition(int x, int y);
    void setCurrentLocation(Location* location);
    // --- Логика ---
    virtual bool isAlive() const;
    virtual void takeDamage(int damage);
    virtual void heal(int amount);
    virtual void gainExperience(int exp);
    virtual void levelUp();
    virtual std::string getType() const = 0;
    // --- Инвентарь ---
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);
    bool hasItem(const std::string& itemName) const;
    // --- Квесты ---
    void addQuest(std::shared_ptr<Quest> quest);
    void completeQuest(std::shared_ptr<Quest> quest);
    // --- ИИ ---
    void setAI(std::unique_ptr<AIBase> newAI);
    AIBase* getAI() const;
    virtual void updateAI(float deltaTime);
    // --- Движение ---
    virtual std::pair<int, int> decideMovement();
    virtual void updateMovement(float deltaTime);
    // --- Обновление ---
    virtual void update(float deltaTime);
    virtual void performAutoAction(float deltaTime) {}
    // Запрет копирования
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
};
#endif // ENTITY_H