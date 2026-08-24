// Systems/SkillSystem.cpp
#include "SkillSystem.h"
#include <iostream>
#include <algorithm>
#include <sstream>

SkillSystem::SkillSystem(int initialSkillPoints)
    : totalSkillPoints(initialSkillPoints), availableSkillPoints(initialSkillPoints) {
    if (initialSkillPoints < 0) {
        totalSkillPoints = 0;
        availableSkillPoints = 0;
    }
}

void SkillSystem::addSkill(const Skill& skill) {
    skills[skill.id] = skill;
}

void SkillSystem::removeSkill(const std::string& skillId) {
    skills.erase(skillId);
}

Skill* SkillSystem::findSkill(const std::string& skillId) {
    auto it = skills.find(skillId);
    if (it != skills.end()) {
        return &(it->second);
    }
    return nullptr;
}

const std::unordered_map<std::string, Skill>& SkillSystem::getSkills() const {
    return skills;
}

std::vector<Skill*> SkillSystem::getSkillsByCategory(SkillCategory category) {
    std::vector<Skill*> result;
    for (auto& pair : skills) {
        if (pair.second.category == category) {
            result.push_back(&(pair.second));
        }
    }
    return result;
}

std::vector<const Skill*> SkillSystem::getSkillsByCategory(SkillCategory category) const {
    std::vector<const Skill*> result;
    for (const auto& pair : skills) {
        if (pair.second.category == category) {
            result.push_back(&(pair.second));
        }
    }
    return result;
}

bool SkillSystem::canUpgradeSkill(const std::string& skillId) const {
    const Skill* skill = const_cast<SkillSystem*>(this)->findSkill(skillId);
    if (!skill || !skill->isUnlocked) {
        return false;
    }
    // Проверяем, достаточно ли очков навыков
    if (availableSkillPoints <= 0) {
        return false;
    }
    return true;
}

bool SkillSystem::upgradeSkill(const std::string& skillId) {
    Skill* skill = findSkill(skillId);
    if (!skill || !skill->isUnlocked) {
        std::cerr << "Ошибка: Навык не найден или заблокирован!" << std::endl;
        return false;
    }
    if (availableSkillPoints <= 0) {
        std::ostringstream oss;
        oss << "Недостаточно очков навыков для прокачки " << skill->name << "!" << "\n";
        std::cout << oss.str();
        return false;
    }
    // Повышаем уровень навыка
    skill->level++;
    skill->experience = 0;
    skill->experienceToNextLevel = skill->level * 100; // Формула для следующего уровня
    // Расходуем очко навыка
    availableSkillPoints--;
    std::ostringstream oss_upgrade;
    oss_upgrade << "Навык " << skill->name << " повышен до уровня " << skill->level << "!" << "\n";
    std::cout << oss_upgrade.str();
    return true;
}

void SkillSystem::gainSkillExperience(const std::string& skillId, int experience) {
    if (experience <= 0) return;
    Skill* skill = findSkill(skillId);
    if (!skill || !skill->isUnlocked) {
        return;
    }
    skill->experience += experience;
    std::ostringstream oss_gain;
    oss_gain << "Получено " << experience << " опыта навыка " << skill->name << "!" << "\n";
    std::cout << oss_gain.str();
    // Проверяем, хватает ли опыта для повышения уровня
    while (skill->experience >= skill->experienceToNextLevel) {
        skill->experience -= skill->experienceToNextLevel;
        skill->level++;
        skill->experienceToNextLevel = skill->level * 100;
        std::ostringstream oss_levelup;
        oss_levelup << "Навык " << skill->name << " автоматически повышен до уровня " << skill->level << "!" << "\n";
        std::cout << oss_levelup.str();
    }
}

int SkillSystem::getTotalSkillPoints() const {
    return totalSkillPoints;
}

int SkillSystem::getAvailableSkillPoints() const {
    return availableSkillPoints;
}

void SkillSystem::addSkillPoints(int points) {
    if (points <= 0) return;
    totalSkillPoints += points;
    availableSkillPoints += points;
    std::ostringstream oss_points;
    oss_points << "Получено " << points << " очков навыков!" << "\n";
    std::cout << oss_points.str();
}

bool SkillSystem::spendSkillPoints(int points) {
    if (points <= 0 || points > availableSkillPoints) {
        return false;
    }
    availableSkillPoints -= points;
    return true;
}

void SkillSystem::checkAutoLevelUp() {
    // Автоматическая проверка всех навыков на повышение уровня
    for (auto& pair : skills) {
        Skill& skill = pair.second;
        if (skill.isUnlocked) {
            while (skill.experience >= skill.experienceToNextLevel) {
                skill.experience -= skill.experienceToNextLevel;
                skill.level++;
                skill.experienceToNextLevel = skill.level * 100;
                std::ostringstream oss_auto;
                oss_auto << "Навык " << skill.name << " повышен до уровня " << skill.level << "!" << "\n";
                std::cout << oss_auto.str();
            }
        }
    }
}

int SkillSystem::getSkillLevel(const std::string& skillId) const {
    const Skill* skill = const_cast<SkillSystem*>(this)->findSkill(skillId);
    if (skill) {
        return skill->level;
    }
    return 0;
}

int SkillSystem::getSkillExperience(const std::string& skillId) const {
    const Skill* skill = const_cast<SkillSystem*>(this)->findSkill(skillId);
    if (skill) {
        return skill->experience;
    }
    return 0;
}

bool SkillSystem::isSkillUnlocked(const std::string& skillId) const {
    const Skill* skill = const_cast<SkillSystem*>(this)->findSkill(skillId);
    if (skill) {
        return skill->isUnlocked;
    }
    return false;
}