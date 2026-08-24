// Systems/AbilitySystem.h

#ifndef ABILITYSYSTEM_H
#define ABILITYSYSTEM_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Forward declarations
class Entity;

enum class AbilityType {
    HEAL,
    DAMAGE,
    BUFF,
    DEBUFF,
    SUMMON
};

struct Ability {
    std::string id;
    std::string name;
    std::string description;
    AbilityType type;
    int manaCost;
    int cooldown;
    int currentCooldown;
    bool isUnlocked;
    Ability() : type(AbilityType::HEAL), manaCost(0), cooldown(0), currentCooldown(0), isUnlocked(false) {}
    Ability(const std::string& id, const std::string& name, const std::string& description,
        AbilityType type, int manaCost, int cooldown)
        : id(id), name(name), description(description), type(type),
        manaCost(manaCost), cooldown(cooldown), currentCooldown(0), isUnlocked(true) {
    }
};

class AbilitySystem {
private:
    std::vector<Ability> abilities;
    std::unordered_map<std::string, int> abilityLevels;
    int totalMana;
    int currentMana;
public:
    // Конструктор
    AbilitySystem(int maxMana = 100);
    // Деструктор
    ~AbilitySystem() = default;
    // Управление способностями
    void addAbility(const Ability& ability);
    void removeAbility(const std::string& abilityId);
    Ability* findAbility(const std::string& abilityId);
    const std::vector<Ability>& getAbilities() const;
    // Использование способностей
    bool canUseAbility(const std::string& abilityId) const;
    bool useAbility(const std::string& abilityId, Entity* caster, Entity* target = nullptr);
    // Управление маной
    int getCurrentMana() const;
    int getTotalMana() const;
    void restoreMana(int amount);
    void consumeMana(int amount);
    // Уровни способностей
    int getAbilityLevel(const std::string& abilityId) const;
    void increaseAbilityLevel(const std::string& abilityId);
    // Обновление системы
    void update(float deltaTime);
    // Запрет копирования
    AbilitySystem(const AbilitySystem&) = delete;
    AbilitySystem& operator=(const AbilitySystem&) = delete;
};
#endif // ABILITYSYSTEM_H