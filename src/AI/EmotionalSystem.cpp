// src/AI/EmotionalSystem.cpp
#include "AI/EmotionalSystem.h"
#include <algorithm>
#include <cmath>

EmotionalSystem::EmotionalSystem(float initialMood, float decayRate)
    : currentMood(initialMood), moodDecayRate(decayRate) {
}

float EmotionalSystem::getMood() const {
    return currentMood;
}

std::string EmotionalSystem::getMoodDescription() const {
    if (currentMood > 0.7f) return "Эйфория";
    if (currentMood > 0.3f) return "Радость";
    if (currentMood > -0.3f) return "Спокойствие";
    if (currentMood > -0.7f) return "Грусть";
    return "Депрессия";
}

void EmotionalSystem::setBaseMood(float mood) {
    currentMood = std::clamp(mood, -1.0f, 1.0f);
}

void EmotionalSystem::setMoodDecayRate(float rate) {
    moodDecayRate = rate;
}

void EmotionalSystem::addMoodModifier(float modifier) {
    currentMood = std::clamp(currentMood + modifier, -1.0f, 1.0f);
}

void EmotionalSystem::update(float deltaTime) {
    if (currentMood > 0.0f) {
        currentMood = std::max(0.0f, currentMood - moodDecayRate * deltaTime);
    }
    else if (currentMood < 0.0f) {
        currentMood = std::min(0.0f, currentMood + moodDecayRate * deltaTime);
    }
}