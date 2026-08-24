// src/AI/EmotionalSystem.h
#ifndef EMOTIONALSYSTEM_H
#define EMOTIONALSYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>

// Расширенная система эмоций с несколькими параметрами
enum class EmotionType {
    JOY,
    FEAR,
    ANGER,
    SADNESS,
    SURPRISE,
    DISGUST,
    TRUST,
    ANTICIPATION
};

struct EmotionState {
    float intensity;      // Интенсивность эмоции (0.0 - 1.0)
    float decayRate;      // Скорость затухания
    float threshold;      // Порог активации
    std::string trigger;  // Триггер эмоции
    
    EmotionState(float i = 0.0f, float d = 0.05f, float t = 0.3f)
        : intensity(i), decayRate(d), threshold(t) {}
};

class EmotionalSystem {
private:
    std::unordered_map<EmotionType, EmotionState> emotions;
    float currentMood;           // Общее настроение (-1.0 до 1.0)
    float moodDecayRate;         // Скорость изменения настроения
    float stressLevel;           // Уровень стресса (0.0 - 1.0)
    float energyLevel;           // Уровень энергии (0.0 - 1.0)
    
    std::vector<std::pair<EmotionType, float>> emotionHistory;
    int maxHistorySize;
    
    // Внутренние методы
    void updateMoodFromEmotions();
    void clampEmotions();
    void addToHistory(EmotionType type, float intensity);
    
public:
    EmotionalSystem(float initialMood = 0.0f, float decayRate = 0.1f);
    ~EmotionalSystem() = default;

    // Основные геттеры
    float getMood() const;
    float getStressLevel() const;
    float getEnergyLevel() const;
    float getEmotionIntensity(EmotionType type) const;
    std::string getMoodDescription() const;
    std::string getDominantEmotion() const;
    
    // Получение всех эмоций
    const std::unordered_map<EmotionType, EmotionState>& getAllEmotions() const;
    
    // Сеттеры
    void setBaseMood(float mood);
    void setMoodDecayRate(float rate);
    void setStressLevel(float stress);
    void setEnergyLevel(float energy);
    
    // Модификаторы эмоций
    void addEmotion(EmotionType type, float intensity);
    void reduceEmotion(EmotionType type, float amount);
    void addMoodModifier(float modifier);
    void addStress(float amount);
    void reduceStress(float amount);
    void addEnergy(float amount);
    void consumeEnergy(float amount);
    
    // Реакция на события
    void onThreatDetected(float threatLevel);
    void onSuccess(float successLevel);
    void onFailure(float failureLevel);
    void onSocialInteraction(bool positive, float intensity);
    void onDiscovery(float interestLevel);
    
    // Обновление системы
    void update(float deltaTime);
    
    // Проверка состояния
    bool isAfraid() const;
    bool isAngry() const;
    bool isHappy() const;
    bool isStressed() const;
    bool canAct() const;  // Проверка достаточности энергии
    
    // Сериализация
    std::string serialize() const;
    void deserialize(const std::string& data);
    
    // Копирование
    EmotionalSystem(const EmotionalSystem&) = delete;
    EmotionalSystem& operator=(const EmotionalSystem&) = delete;
};

#endif // EMOTIONALSYSTEM_H