// src/AI/EmotionalSystem.cpp
#include "AI/EmotionalSystem.h"
#include <algorithm>
#include <cmath>
#include <sstream>

EmotionalSystem::EmotionalSystem(float initialMood, float decayRate)
    : currentMood(initialMood), moodDecayRate(decayRate),
      stressLevel(0.0f), energyLevel(1.0f), maxHistorySize(20) {
    // Инициализация всех эмоций
    emotions[EmotionType::JOY] = EmotionState(0.0f, 0.05f, 0.3f);
    emotions[EmotionType::FEAR] = EmotionState(0.0f, 0.08f, 0.3f);
    emotions[EmotionType::ANGER] = EmotionState(0.0f, 0.06f, 0.3f);
    emotions[EmotionType::SADNESS] = EmotionState(0.0f, 0.04f, 0.3f);
    emotions[EmotionType::SURPRISE] = EmotionState(0.0f, 0.10f, 0.3f);
    emotions[EmotionType::DISGUST] = EmotionState(0.0f, 0.07f, 0.3f);
    emotions[EmotionType::TRUST] = EmotionState(0.0f, 0.03f, 0.3f);
    emotions[EmotionType::ANTICIPATION] = EmotionState(0.0f, 0.05f, 0.3f);
}

float EmotionalSystem::getMood() const {
    return currentMood;
}

float EmotionalSystem::getStressLevel() const {
    return stressLevel;
}

float EmotionalSystem::getEnergyLevel() const {
    return energyLevel;
}

float EmotionalSystem::getEmotionIntensity(EmotionType type) const {
    auto it = emotions.find(type);
    if (it != emotions.end()) {
        return it->second.intensity;
    }
    return 0.0f;
}

const std::unordered_map<EmotionType, EmotionState>& EmotionalSystem::getAllEmotions() const {
    return emotions;
}

std::string EmotionalSystem::getMoodDescription() const {
    if (currentMood > 0.7f) return "Эйфория";
    if (currentMood > 0.3f) return "Радость";
    if (currentMood > -0.3f) return "Спокойствие";
    if (currentMood > -0.7f) return "Грусть";
    return "Депрессия";
}

std::string EmotionalSystem::getDominantEmotion() const {
    EmotionType dominant = EmotionType::JOY;
    float maxIntensity = 0.0f;
    
    for (const auto& [type, state] : emotions) {
        if (state.intensity > maxIntensity) {
            maxIntensity = state.intensity;
            dominant = type;
        }
    }
    
    if (maxIntensity < 0.3f) return "Нейтрально";
    
    switch (dominant) {
        case EmotionType::JOY: return "Радость";
        case EmotionType::FEAR: return "Страх";
        case EmotionType::ANGER: return "Гнев";
        case EmotionType::SADNESS: return "Грусть";
        case EmotionType::SURPRISE: return "Удивление";
        case EmotionType::DISGUST: return "Отвращение";
        case EmotionType::TRUST: return "Доверие";
        case EmotionType::ANTICIPATION: return "Ожидание";
        default: return "Нейтрально";
    }
}

void EmotionalSystem::setBaseMood(float mood) {
    currentMood = std::clamp(mood, -1.0f, 1.0f);
}

void EmotionalSystem::setMoodDecayRate(float rate) {
    moodDecayRate = rate;
}

void EmotionalSystem::setStressLevel(float stress) {
    stressLevel = std::clamp(stress, 0.0f, 1.0f);
}

void EmotionalSystem::setEnergyLevel(float energy) {
    energyLevel = std::clamp(energy, 0.0f, 1.0f);
}

void EmotionalSystem::addEmotion(EmotionType type, float intensity) {
    auto it = emotions.find(type);
    if (it != emotions.end()) {
        it->second.intensity = std::min(1.0f, it->second.intensity + intensity);
        it->second.trigger = "recent_event";
        addToHistory(type, it->second.intensity);
        updateMoodFromEmotions();
    }
}

void EmotionalSystem::reduceEmotion(EmotionType type, float amount) {
    auto it = emotions.find(type);
    if (it != emotions.end()) {
        it->second.intensity = std::max(0.0f, it->second.intensity - amount);
    }
}

void EmotionalSystem::addMoodModifier(float modifier) {
    currentMood = std::clamp(currentMood + modifier, -1.0f, 1.0f);
}

void EmotionalSystem::addStress(float amount) {
    stressLevel = std::min(1.0f, stressLevel + amount);
    // Высокий стресс усиливает страх и гнев
    if (stressLevel > 0.7f) {
        addEmotion(EmotionType::FEAR, amount * 0.5f);
        addEmotion(EmotionType::ANGER, amount * 0.3f);
    }
}

void EmotionalSystem::reduceStress(float amount) {
    stressLevel = std::max(0.0f, stressLevel - amount);
    if (stressLevel < 0.3f) {
        reduceEmotion(EmotionType::FEAR, 0.1f);
        reduceEmotion(EmotionType::ANGER, 0.1f);
    }
}

void EmotionalSystem::addEnergy(float amount) {
    energyLevel = std::min(1.0f, energyLevel + amount);
}

void EmotionalSystem::consumeEnergy(float amount) {
    energyLevel = std::max(0.0f, energyLevel - amount);
    if (energyLevel < 0.2f) {
        addEmotion(EmotionType::SADNESS, 0.2f);
        addEmotion(EmotionType::ANGER, 0.1f);
    }
}

void EmotionalSystem::onThreatDetected(float threatLevel) {
    addEmotion(EmotionType::FEAR, threatLevel * 0.8f);
    addEmotion(EmotionType::ANGER, threatLevel * 0.4f);
    addStress(threatLevel * 0.6f);
    consumeEnergy(threatLevel * 0.3f);
}

void EmotionalSystem::onSuccess(float successLevel) {
    addEmotion(EmotionType::JOY, successLevel * 0.7f);
    addEmotion(EmotionType::TRUST, successLevel * 0.4f);
    addMoodModifier(successLevel * 0.5f);
    reduceStress(successLevel * 0.3f);
}

void EmotionalSystem::onFailure(float failureLevel) {
    addEmotion(EmotionType::SADNESS, failureLevel * 0.6f);
    addEmotion(EmotionType::ANGER, failureLevel * 0.3f);
    addMoodModifier(-failureLevel * 0.4f);
    addStress(failureLevel * 0.4f);
}

void EmotionalSystem::onSocialInteraction(bool positive, float intensity) {
    if (positive) {
        addEmotion(EmotionType::JOY, intensity * 0.6f);
        addEmotion(EmotionType::TRUST, intensity * 0.8f);
        addMoodModifier(intensity * 0.4f);
        reduceStress(intensity * 0.3f);
    } else {
        addEmotion(EmotionType::SADNESS, intensity * 0.5f);
        addEmotion(EmotionType::ANGER, intensity * 0.3f);
        addEmotion(EmotionType::DISGUST, intensity * 0.4f);
        addMoodModifier(-intensity * 0.3f);
        addStress(intensity * 0.4f);
    }
}

void EmotionalSystem::onDiscovery(float interestLevel) {
    addEmotion(EmotionType::SURPRISE, interestLevel * 0.7f);
    addEmotion(EmotionType::ANTICIPATION, interestLevel * 0.6f);
    addEmotion(EmotionType::JOY, interestLevel * 0.4f);
    addMoodModifier(interestLevel * 0.3f);
}

void EmotionalSystem::updateMoodFromEmotions() {
    float positiveSum = 0.0f;
    float negativeSum = 0.0f;
    
    // Положительные эмоции
    positiveSum += emotions[EmotionType::JOY].intensity * 1.0f;
    positiveSum += emotions[EmotionType::TRUST].intensity * 0.8f;
    positiveSum += emotions[EmotionType::ANTICIPATION].intensity * 0.6f;
    positiveSum += emotions[EmotionType::SURPRISE].intensity * 0.3f;
    
    // Отрицательные эмоции
    negativeSum += emotions[EmotionType::SADNESS].intensity * 1.0f;
    negativeSum += emotions[EmotionType::FEAR].intensity * 0.9f;
    negativeSum += emotions[EmotionType::ANGER].intensity * 0.8f;
    negativeSum += emotions[EmotionType::DISGUST].intensity * 0.7f;
    
    // Влияние стресса и энергии
    float stressModifier = -stressLevel * 0.3f;
    float energyModifier = (energyLevel - 0.5f) * 0.2f;
    
    currentMood = std::clamp(
        (positiveSum - negativeSum) * 0.5f + stressModifier + energyModifier,
        -1.0f, 1.0f
    );
}

void EmotionalSystem::clampEmotions() {
    for (auto& [type, state] : emotions) {
        state.intensity = std::clamp(state.intensity, 0.0f, 1.0f);
    }
    stressLevel = std::clamp(stressLevel, 0.0f, 1.0f);
    energyLevel = std::clamp(energyLevel, 0.0f, 1.0f);
}

void EmotionalSystem::addToHistory(EmotionType type, float intensity) {
    emotionHistory.push_back({type, intensity});
    if (emotionHistory.size() > static_cast<size_t>(maxHistorySize)) {
        emotionHistory.erase(emotionHistory.begin());
    }
}

void EmotionalSystem::update(float deltaTime) {
    // Затухание эмоций
    for (auto& [type, state] : emotions) {
        if (state.intensity > 0.0f) {
            state.intensity = std::max(0.0f, state.intensity - state.decayRate * deltaTime);
        }
    }
    
    // Возврат настроения к нейтральному
    if (currentMood > 0.0f) {
        currentMood = std::max(0.0f, currentMood - moodDecayRate * deltaTime);
    } else if (currentMood < 0.0f) {
        currentMood = std::min(0.0f, currentMood + moodDecayRate * deltaTime);
    }
    
    // Снижение стресса со временем
    if (stressLevel > 0.0f) {
        stressLevel = std::max(0.0f, stressLevel - 0.02f * deltaTime);
    }
    
    // Восстановление энергии
    if (energyLevel < 1.0f) {
        energyLevel = std::min(1.0f, energyLevel + 0.01f * deltaTime);
    }
    
    clampEmotions();
}

bool EmotionalSystem::isAfraid() const {
    return emotions.at(EmotionType::FEAR).intensity > 0.5f;
}

bool EmotionalSystem::isAngry() const {
    return emotions.at(EmotionType::ANGER).intensity > 0.5f;
}

bool EmotionalSystem::isHappy() const {
    return emotions.at(EmotionType::JOY).intensity > 0.5f || currentMood > 0.5f;
}

bool EmotionalSystem::isStressed() const {
    return stressLevel > 0.7f;
}

bool EmotionalSystem::canAct() const {
    return energyLevel > 0.1f && !isStressed();
}

std::string EmotionalSystem::serialize() const {
    std::ostringstream oss;
    oss << currentMood << "," << moodDecayRate << "," 
        << stressLevel << "," << energyLevel << ",";
    
    for (const auto& [type, state] : emotions) {
        oss << static_cast<int>(type) << ":" << state.intensity << ";";
    }
    
    return oss.str();
}

void EmotionalSystem::deserialize(const std::string& data) {
    std::istringstream iss(data);
    char comma, colon, semicolon;
    
    iss >> currentMood >> comma >> moodDecayRate >> comma 
        >> stressLevel >> comma >> energyLevel >> comma;
    
    int typeInt;
    float intensity;
    while (iss >> typeInt >> colon >> intensity >> semicolon) {
        EmotionType type = static_cast<EmotionType>(typeInt);
        if (emotions.find(type) != emotions.end()) {
            emotions[type].intensity = intensity;
        }
    }
}