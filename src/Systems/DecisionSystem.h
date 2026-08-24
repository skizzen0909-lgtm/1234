// Systems/DecisionSystem.h
#ifndef DECISIONSYSTEM_H
#define DECISIONSYSTEM_H

#include <vector>
#include <memory>
#include <string>

// Forward declarations

class Decision;
class Entity;
class Event;

class DecisionSystem {
private:
    std::vector<std::shared_ptr<Decision>> availableDecisions;

public:
    // Конструктор/деструктор
    DecisionSystem();
    ~DecisionSystem() = default;

    // Управление решениями
    void addDecision(std::shared_ptr<Decision> decision);
    void removeDecision(const std::shared_ptr<Decision>& decision);
    std::shared_ptr<Decision> findDecisionById(const std::string& id) const;
    const std::vector<std::shared_ptr<Decision>>& getAvailableDecisions() const;

    // Методы системы
    void presentDecisions(Entity* decisionMaker) const;
    void executeDecision(std::shared_ptr<Decision> decision, Entity* decisionMaker);

    // Запрет копирования
    DecisionSystem(const DecisionSystem&) = delete;
    DecisionSystem& operator=(const DecisionSystem&) = delete;
};

#endif // DECISIONSYSTEM_H