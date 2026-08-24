// Systems/DecisionSystem.cpp
#include "DecisionSystem.h"
#include "Decisions/Decision.h"
#include "Entities/Entity.h"
#include "UI/DialogSystem.h"
#include "Events/Event.h"
#include <iostream>
#include <algorithm>
#include <memory>
DecisionSystem::DecisionSystem() {
}

void DecisionSystem::addDecision(std::shared_ptr<Decision> decision) {
    if (decision) {
        availableDecisions.push_back(decision);
    }
}

void DecisionSystem::removeDecision(const std::shared_ptr<Decision>& decision) {
    if (!decision) return;
    auto it = std::find(availableDecisions.begin(), availableDecisions.end(), decision);
    if (it != availableDecisions.end()) {
        availableDecisions.erase(it);
    }
}

std::shared_ptr<Decision> DecisionSystem::findDecisionById(const std::string& id) const {
    auto it = std::find_if(availableDecisions.begin(), availableDecisions.end(),
        [&id](const std::shared_ptr<Decision>& decision) {
            return decision && decision->getDecisionId() == id;
        });
    if (it != availableDecisions.end()) {
        return *it;
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Decision>>& DecisionSystem::getAvailableDecisions() const {
    return availableDecisions;
}

void DecisionSystem::presentDecisions(Entity* decisionMaker) const {
    if (!decisionMaker) return;
    std::cout << "\n=== Доступные решения для " << decisionMaker->getName() << " ===" << std::endl;
    for (size_t i = 0; i < availableDecisions.size(); ++i) {
        const auto& decision = availableDecisions[i];
        if (decision) {
            std::cout << (i + 1) << ". " << decision->getDescription() << std::endl;
        }
    }
    std::cout << "========================" << std::endl;
}

void DecisionSystem::executeDecision(std::shared_ptr<Decision> decision, Entity* decisionMaker) {
    if (!decision || !decisionMaker) {
        return;
    }

    // Выполняем все последствия решения
    const auto& consequences = decision->getConsequences();
    for (const auto& consequence : consequences) {
        if (consequence) {
            consequence->execute();
        }
    }

    // Опционально: можно логировать
    std::cout << decisionMaker->getName() << " выбрал решение: \"" << decision->getDescription() << "\"\n";
}