// src/AI/LearningSystem.h
#ifndef LEARNINGSYSTEM_H
#define LEARNINGSYSTEM_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

class Entity;

// Структура для хранения опыта
struct LearningExperience {
    std::string situationType;      // Тип ситуации (например, "combat", "social", "exploration")
    std::string actionTaken;        // Действие, которое было предпринято
    float outcomeValue;             // Результат действия (-1.0 до 1.0, где 1.0 - отлично)
    int experienceCount;            // Сколько раз повторялось
    float confidence;               // Уверенность в этом опыте (0.0 - 1.0)
    
    LearningExperience() 
        : situationType(""), actionTaken(""), outcomeValue(0.0f),
          experienceCount(0), confidence(0.5f) {}
    
    LearningExperience(const std::string& situation, const std::string& action, 
                       float outcome = 0.0f)
        : situationType(situation), actionTaken(action), outcomeValue(outcome),
          experienceCount(1), confidence(0.5f) {}
};

// Структура для паттернов поведения
struct BehaviorPattern {
    std::string patternId;
    std::string triggerCondition;   // Условие активации
    std::vector<std::string> actions; // Список действий
    float successRate;              // Процент успешных применений
    int usageCount;                 // Сколько раз использовалось
    
    BehaviorPattern() 
        : patternId(""), triggerCondition(""), successRate(0.5f), usageCount(0) {}
    
    BehaviorPattern(const std::string& id, const std::string& trigger)
        : patternId(id), triggerCondition(trigger), successRate(0.5f), usageCount(0) {}
};

// Система обучения на основе опыта
class LearningSystem {
private:
    std::unordered_map<std::string, std::vector<LearningExperience>> experienceLog;
    std::unordered_map<std::string, BehaviorPattern> behaviorPatterns;
    
    float learningRate;             // Скорость обучения (0.0 - 1.0)
    float memoryDecay;              // Скорость забывания старого опыта
    int maxExperiencesPerType;      // Максимум опытов на тип ситуации
    
    // Внутренние методы
    void updateConfidence(LearningExperience& exp);
    void mergeSimilarExperiences();
    void generateBehaviorPattern(const std::string& situationType);
    
public:
    LearningSystem(float learnRate = 0.3f, float decay = 0.01f, int maxExp = 50);
    ~LearningSystem() = default;
    
    // Добавление нового опыта
    void addExperience(const std::string& situation, const std::string& action, float outcome);
    
    // Получение лучшего действия для ситуации
    std::string getBestAction(const std::string& situationType) const;
    
    // Получение всех действий для ситуации с весами
    std::vector<std::pair<std::string, float>> getWeightedActions(const std::string& situationType) const;
    
    // Обновление паттернов поведения
    void updateBehaviorPatterns();
    
    // Применение паттерна
    bool applyBehaviorPattern(const std::string& patternId, Entity* entity);
    
    // Статистика обучения
    float getSuccessRate(const std::string& situationType) const;
    int getTotalExperiences() const;
    const std::vector<LearningExperience>& getExperiencesForSituation(const std::string& type) const;
    
    // Сериализация/десериализация
    std::string serialize() const;
    void deserialize(const std::string& data);
    
    // Геттеры/сеттеры
    float getLearningRate() const { return learningRate; }
    void setLearningRate(float rate) { learningRate = rate; }
    float getMemoryDecay() const { return memoryDecay; }
    void setMemoryDecay(float decay) { memoryDecay = decay; }
};

#endif // LEARNINGSYSTEM_H
