// src/AI/LearningSystem.cpp
#include "AI/LearningSystem.h"
#include "Entities/Entity.h"
#include <algorithm>
#include <sstream>
#include <cmath>
#include <iostream>

LearningSystem::LearningSystem(float learnRate, float decay, int maxExp)
    : learningRate(learnRate), memoryDecay(decay), maxExperiencesPerType(maxExp) {
}

void LearningSystem::updateConfidence(LearningExperience& exp) {
    // Уверенность растёт с количеством повторений и хорошим результатом
    float countFactor = std::min(1.0f, exp.experienceCount / 10.0f);
    float outcomeFactor = (exp.outcomeValue + 1.0f) / 2.0f; // Нормализуем от 0 до 1
    exp.confidence = 0.5f * countFactor + 0.5f * outcomeFactor;
}

void LearningSystem::mergeSimilarExperiences() {
    for (auto& [situation, experiences] : experienceLog) {
        if (experiences.size() > static_cast<size_t>(maxExperiencesPerType)) {
            // Сортируем по уверенности и оставляем только лучшие
            std::sort(experiences.begin(), experiences.end(),
                [](const LearningExperience& a, const LearningExperience& b) {
                    return a.confidence > b.confidence;
                });
            experiences.resize(maxExperiencesPerType);
        }
    }
}

void LearningSystem::generateBehaviorPattern(const std::string& situationType) {
    auto it = experienceLog.find(situationType);
    if (it == experienceLog.end() || it->second.empty()) return;
    
    // Находим наиболее успешные действия
    std::vector<std::pair<std::string, float>> actionScores;
    for (const auto& exp : it->second) {
        bool found = false;
        for (auto& [action, score] : actionScores) {
            if (action == exp.actionTaken) {
                score += exp.outcomeValue;
                found = true;
                break;
            }
        }
        if (!found) {
            actionScores.emplace_back(exp.actionTaken, exp.outcomeValue);
        }
    }
    
    // Сортируем по успешности
    std::sort(actionScores.begin(), actionScores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Создаём паттерн
    std::string patternId = "pattern_" + situationType;
    BehaviorPattern pattern(patternId, situationType);
    
    for (size_t i = 0; i < std::min(actionScores.size(), size_t(3)); ++i) {
        if (actionScores[i].second > 0.0f) {
            pattern.actions.push_back(actionScores[i].first);
        }
    }
    
    if (!pattern.actions.empty()) {
        pattern.successRate = pattern.actions.size() / 3.0f;
        behaviorPatterns[patternId] = pattern;
    }
}

void LearningSystem::addExperience(const std::string& situation, const std::string& action, float outcome) {
    outcome = std::clamp(outcome, -1.0f, 1.0f);
    
    auto& experiences = experienceLog[situation];
    
    // Проверяем, есть ли уже похожий опыт
    bool found = false;
    for (auto& exp : experiences) {
        if (exp.actionTaken == action) {
            // Обновляем существующий опыт
            exp.outcomeValue = (1.0f - learningRate) * exp.outcomeValue + learningRate * outcome;
            exp.experienceCount++;
            updateConfidence(exp);
            found = true;
            break;
        }
    }
    
    if (!found) {
        // Добавляем новый опыт
        experiences.emplace_back(situation, action, outcome);
        updateConfidence(experiences.back());
    }
    
    // Генерируем паттерны периодически
    if (experiences.size() % 5 == 0) {
        generateBehaviorPattern(situation);
    }
    
    mergeSimilarExperiences();
}

std::string LearningSystem::getBestAction(const std::string& situationType) const {
    auto it = experienceLog.find(situationType);
    if (it == experienceLog.end() || it->second.empty()) {
        return "";
    }
    
    // Находим действие с наибольшей уверенностью и результатом
    const LearningExperience* bestExp = nullptr;
    float bestScore = -2.0f;
    
    for (const auto& exp : it->second) {
        float score = exp.confidence * 0.5f + ((exp.outcomeValue + 1.0f) / 2.0f) * 0.5f;
        if (score > bestScore) {
            bestScore = score;
            bestExp = &exp;
        }
    }
    
    return bestExp ? bestExp->actionTaken : "";
}

std::vector<std::pair<std::string, float>> LearningSystem::getWeightedActions(const std::string& situationType) const {
    std::vector<std::pair<std::string, float>> result;
    
    auto it = experienceLog.find(situationType);
    if (it == experienceLog.end()) {
        return result;
    }
    
    for (const auto& exp : it->second) {
        float weight = exp.confidence * ((exp.outcomeValue + 1.0f) / 2.0f);
        result.emplace_back(exp.actionTaken, weight);
    }
    
    // Сортируем по весу
    std::sort(result.begin(), result.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return result;
}

void LearningSystem::updateBehaviorPatterns() {
    for (const auto& [situation, experiences] : experienceLog) {
        generateBehaviorPattern(situation);
    }
}

bool LearningSystem::applyBehaviorPattern(const std::string& patternId, Entity* entity) {
    if (!entity) return false;
    
    auto it = behaviorPatterns.find(patternId);
    if (it == behaviorPatterns.end() || it->second.actions.empty()) {
        return false;
    }
    
    // Выбираем случайное действие из паттерна с учётом успешности
    const auto& actions = it->second.actions;
    size_t index = 0;
    if (actions.size() > 1) {
        // Простая рандомизация - можно улучшить взвешенным выбором
        index = std::hash<std::string>{}(patternId + std::to_string(it->second.usageCount)) % actions.size();
    }
    
    std::cout << entity->getName() << " применяет паттерн '" << patternId 
              << "': " << actions[index] << std::endl;
    
    it->second.usageCount++;
    return true;
}

float LearningSystem::getSuccessRate(const std::string& situationType) const {
    auto it = experienceLog.find(situationType);
    if (it == experienceLog.end() || it->second.empty()) {
        return 0.5f; // По умолчанию
    }
    
    float totalOutcome = 0.0f;
    for (const auto& exp : it->second) {
        totalOutcome += exp.outcomeValue;
    }
    
    return (totalOutcome / it->second.size() + 1.0f) / 2.0f; // Нормализуем к 0-1
}

int LearningSystem::getTotalExperiences() const {
    int total = 0;
    for (const auto& [_, experiences] : experienceLog) {
        total += static_cast<int>(experiences.size());
    }
    return total;
}

const std::vector<LearningExperience>& LearningSystem::getExperiencesForSituation(const std::string& type) const {
    static const std::vector<LearningExperience> empty;
    auto it = experienceLog.find(type);
    return it != experienceLog.end() ? it->second : empty;
}

std::string LearningSystem::serialize() const {
    std::ostringstream oss;
    
    // Сериализуем опыт
    oss << "EXPERIENCES:";
    for (const auto& [situation, experiences] : experienceLog) {
        for (const auto& exp : experiences) {
            oss << "\n" << situation << "|" << exp.actionTaken << "|" 
                << exp.outcomeValue << "|" << exp.experienceCount << "|" 
                << exp.confidence;
        }
    }
    
    // Сериализуем паттерны
    oss << "\nPATTERNS:";
    for (const auto& [id, pattern] : behaviorPatterns) {
        oss << "\n" << id << "|" << pattern.triggerCondition << "|" << pattern.successRate;
        for (const auto& action : pattern.actions) {
            oss << ";" << action;
        }
    }
    
    oss << "\nSETTINGS:" << learningRate << "|" << memoryDecay << "|" << maxExperiencesPerType;
    
    return oss.str();
}

void LearningSystem::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    std::string currentSection;
    
    while (std::getline(iss, line)) {
        if (line == "EXPERIENCES:") {
            currentSection = "experiences";
            continue;
        } else if (line == "PATTERNS:") {
            currentSection = "patterns";
            continue;
        } else if (line == "SETTINGS:") {
            currentSection = "settings";
            continue;
        }
        
        if (currentSection == "experiences" && !line.empty()) {
            std::istringstream linestream(line);
            std::string situation, action;
            float outcome, confidence;
            int count;
            
            if (std::getline(linestream, situation, '|') &&
                std::getline(linestream, action, '|') &&
                linestream >> outcome && linestream.ignore() >> count && linestream.ignore() >> confidence) {
                
                LearningExperience exp(situation, action, outcome);
                exp.experienceCount = count;
                exp.confidence = confidence;
                experienceLog[situation].push_back(exp);
            }
        }
        // Парсинг паттернов и настроек можно добавить при необходимости
    }
}
