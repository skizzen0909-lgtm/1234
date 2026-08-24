// Systems/AbilitySystem.cpp
#include "AbilitySystem.h"
#include "Entities/Entity.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <locale>  // Для setlocale
#include <sstream> // Для std::ostringstream

AbilitySystem::AbilitySystem(int maxMana)
    : totalMana(maxMana), currentMana(maxMana) {
    // Установка локали для корректного отображения русского текста
    setlocale(LC_ALL, "Russian");
    if (maxMana < 0) {
        totalMana = 100;
        currentMana = 100;
    }
}

void AbilitySystem::addAbility(const Ability& ability) {
    // Проверяем, не существует ли уже такая способность
    auto it = std::find_if(abilities.begin(), abilities.end(),
        [&ability](const Ability& a) { return a.id == ability.id; });
    if (it == abilities.end()) {
        abilities.push_back(ability);
        abilityLevels[ability.id] = 1;
    }
}

void AbilitySystem::removeAbility(const std::string& abilityId) {
    auto it = std::find_if(abilities.begin(), abilities.end(),
        [&abilityId](const Ability& a) { return a.id == abilityId; });
    if (it != abilities.end()) {
        abilities.erase(it);
        abilityLevels.erase(abilityId);
    }
}

Ability* AbilitySystem::findAbility(const std::string& abilityId) {
    auto it = std::find_if(abilities.begin(), abilities.end(),
        [&abilityId](const Ability& a) { return a.id == abilityId; });
    if (it != abilities.end()) {
        return &(*it);
    }
    return nullptr;
}

const std::vector<Ability>& AbilitySystem::getAbilities() const {
    return abilities;
}

bool AbilitySystem::canUseAbility(const std::string& abilityId) const {
    const Ability* ability = const_cast<AbilitySystem*>(this)->findAbility(abilityId);
    if (!ability || !ability->isUnlocked) {
        return false;
    }
    // Проверяем кулдаун
    if (ability->currentCooldown > 0) {
        return false;
    }
    // Проверяем ману
    if (currentMana < ability->manaCost) {
        return false;
    }
    return true;
}

bool AbilitySystem::useAbility(const std::string& abilityId, Entity* caster, Entity* target) {
    if (!caster) {
        std::cerr << "Ошибка: Невозможно использовать способность без заклинателя!" << std::endl;
        return false;
    }
    Ability* ability = findAbility(abilityId);
    if (!ability || !ability->isUnlocked) {
        std::cerr << "Ошибка: Способность не найдена или заблокирована!" << std::endl;
        return false;
    }
    // Проверяем кулдаун
    if (ability->currentCooldown > 0) {
        std::ostringstream oss;
        oss << "Способность " << ability->name << " еще не готова!";
        std::cout << oss.str() << std::endl;
        return false;
    }
    // Проверяем ману
    if (currentMana < ability->manaCost) {
        std::ostringstream oss;
        oss << "Недостаточно маны для использования " << ability->name << "!" << "\n";
        std::cout << oss.str();
        return false;
    }
    // Используем способность
    std::ostringstream oss;
    oss << caster->getName() << " использует " << ability->name << "!" << "\n";
    std::cout << oss.str();

    // Логика использования способности
    switch (ability->type) {
    case AbilityType::HEAL:
        if (target) {
            // Восстанавливаем здоровье цели
            target->heal(50); // Пример значения
            std::ostringstream healOss;
            healOss << target->getName() << " восстанавливает здоровье!" << std::endl;
            std::cout << healOss.str();
        }
        else {
            // Восстанавливаем здоровье заклинателя
            caster->heal(50); // Пример значения
            std::ostringstream healOss;
            healOss << caster->getName() << " восстанавливает свое здоровье!" << std::endl;
            std::cout << healOss.str();
        }
        break;
    case AbilityType::DAMAGE:
        if (target) {
            // Наносим урон цели
            target->takeDamage(30); // Пример значения
            std::ostringstream damageOss;
            damageOss << target->getName() << " получает урон!" << std::endl;
            std::cout << damageOss.str();
        }
        break;
    case AbilityType::BUFF:
    {
        std::ostringstream buffOss;
        buffOss << "Наложен бафф на " << (target ? target->getName() : caster->getName()) << "!" << std::endl;
        std::cout << buffOss.str();
    }
    break;
    case AbilityType::DEBUFF:
        if (target) {
            std::ostringstream debuffOss;
            debuffOss << "Наложен дебафф на " << target->getName() << "!" << std::endl;
            std::cout << debuffOss.str();
        }
        break;
    case AbilityType::SUMMON:
        std::cout << "Призвано существо!" << std::endl;
        break;
    }
    // Расходуем ману
    consumeMana(ability->manaCost);
    // Устанавливаем кулдаун
    ability->currentCooldown = ability->cooldown;
    return true;
}

int AbilitySystem::getCurrentMana() const {
    return currentMana;
}

int AbilitySystem::getTotalMana() const {
    return totalMana;
}

void AbilitySystem::restoreMana(int amount) {
    if (amount <= 0) return;
    currentMana += amount;
    if (currentMana > totalMana) {
        currentMana = totalMana;
    }
}

void AbilitySystem::consumeMana(int amount) {
    if (amount <= 0) return;
    currentMana -= amount;
    if (currentMana < 0) {
        currentMana = 0;
    }
}

int AbilitySystem::getAbilityLevel(const std::string& abilityId) const {
    auto it = abilityLevels.find(abilityId);
    if (it != abilityLevels.end()) {
        return it->second;
    }
    return 0;
}

void AbilitySystem::increaseAbilityLevel(const std::string& abilityId) {
    Ability* ability = findAbility(abilityId);
    if (ability) {
        abilityLevels[abilityId]++;
        std::ostringstream oss;
        oss << "Уровень способности " << ability->name << " повышен до " << abilityLevels[abilityId] << "!";
        std::cout << oss.str() << std::endl;
    }
}

void AbilitySystem::update(float deltaTime) {
    // Обновляем кулдауны
    for (auto& ability : abilities) {
        if (ability.currentCooldown > 0) {
            ability.currentCooldown--;
        }
    }
    // Медленно восстанавливаем ману (пример: 1 мана в секунду)
    static float manaRegenTimer = 0.0f;
    manaRegenTimer += deltaTime;
    if (manaRegenTimer >= 1.0f) {
        restoreMana(1);
        manaRegenTimer = 0.0f;
    }
}