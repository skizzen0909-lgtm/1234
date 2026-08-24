// Decisions/Decision.h
#ifndef DECISION_H
#define DECISION_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Entity;
class Event;

class Decision {
private:
    std::string decisionId;
    std::string description;
    std::vector<std::shared_ptr<Event>> consequences;

public:
    // Конструктор
    Decision(const std::string& decisionId, const std::string& description);

    // Деструктор
    virtual ~Decision() = default;

    // Геттеры
    const std::string& getDecisionId() const;
    const std::string& getDescription() const;
    const std::vector<std::shared_ptr<Event>>& getConsequences() const;

    // Сеттеры
    void setDescription(const std::string& description);

    // Управление последствиями
    void addConsequence(std::shared_ptr<Event> event);
    void removeConsequence(const std::shared_ptr<Event>& event);

    // Методы принятия решения
    

    // Запрет копирования
    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
};

#endif // DECISION_H