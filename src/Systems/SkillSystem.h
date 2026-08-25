// Systems/SkillSystem.h
#ifndef SKILLSYSTEM_H
#define SKILLSYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declarations
class Entity;

enum class SkillCategory {
    COMBAT,
    MAGIC,
    CRAFTING,
    SOCIAL,
    SURVIVAL
};

struct Skill {
    std::string id;
    std::string name;
    std::string description;
    SkillCategory category;
    int level;
    int experience;
    int experienceToNextLevel;
    bool isUnlocked;

    Skill() : category(SkillCategory::COMBAT), level(1), experience(0),
        experienceToNextLevel(100), isUnlocked(false) {
    }

    Skill(const std::string& id, const std::string& name, const std::string& description,
        SkillCategory category, int startingLevel = 1)
        : id(id), name(name), description(description), category(category),
        level(startingLevel), experience(0), experienceToNextLevel(startingLevel * 100),
        isUnlocked(true) {
    }
};

class SkillSystem {
private:
    std::unordered_map<std::string, Skill> skills;
    int totalSkillPoints;
    int availableSkillPoints;

public:
    // Конструктор
    SkillSystem(int initialSkillPoints = 0);

    // Деструктор
    ~SkillSystem() = default;

    // Управление навыками
    void addSkill(const Skill& skill);
    void removeSkill(const std::string& skillId);
    Skill* findSkill(const std::string& skillId);
    const std::unordered_map<std::string, Skill>& getSkills() const;

    // Получение навыков по категории
    std::vector<Skill*> getSkillsByCategory(SkillCategory category);
    std::vector<const Skill*> getSkillsByCategory(SkillCategory category) const;

    // Прокачка навыков
    bool canUpgradeSkill(const std::string& skillId) const;
    bool upgradeSkill(const std::string& skillId);
    void gainSkillExperience(const std::string& skillId, int experience);

    // Управление очками навыков
    int getTotalSkillPoints() const;
    int getAvailableSkillPoints() const;
    void addSkillPoints(int points);
    bool spendSkillPoints(int points);

    // Автоматическая прокачка
    void checkAutoLevelUp();

    // Получение информации
    int getSkillLevel(const std::string& skillId) const;
    int getSkillExperience(const std::string& skillId) const;
    bool isSkillUnlocked(const std::string& skillId) const;

    // Запрет копирования
    SkillSystem(const SkillSystem&) = delete;
    SkillSystem& operator=(const SkillSystem&) = delete;
};

#endif // SKILLSYSTEM_H