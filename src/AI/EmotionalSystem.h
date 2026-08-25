// src/AI/EmotionalSystem.h
#ifndef EMOTIONALSYSTEM_H
#define EMOTIONALSYSTEM_H

#include <string> 

class EmotionalSystem {
private:
    float currentMood;
    float moodDecayRate;

public:
    EmotionalSystem(float initialMood = 0.0f, float decayRate = 0.1f);
    ~EmotionalSystem() = default;

    float getMood() const;
    std::string getMoodDescription() const; // Объявлен

    void setBaseMood(float mood);
    void setMoodDecayRate(float rate);

    void addMoodModifier(float modifier);
    void update(float deltaTime);

    EmotionalSystem(const EmotionalSystem&) = delete;
    EmotionalSystem& operator=(const EmotionalSystem&) = delete;
};

#endif // EMOTIONALSYSTEM_H