// src/Events/Event.h
#ifndef EVENT_H
#define EVENT_H


#include <memory>
#include <string>

// Forward declarations
class Entity;

class Event {
protected:
    std::string eventId;
    bool handled;

public:
    // Конструктор
    explicit Event(const std::string& eventId);

    // Виртуальный деструктор - КРИТИЧНО ВАЖНО
    virtual ~Event() = default;

    // Геттеры
    const std::string& getEventId() const;
    bool isHandled() const;

    // Сеттеры
    void setHandled(bool handled);

    // Виртуальные методы
    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;

    // Запрет копирования
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;
};

#endif // EVENT_H