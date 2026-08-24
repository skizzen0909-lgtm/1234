// GameState/GameState.h
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <memory>
#include <string>

class GameState {
protected:
    std::string stateId;

public:
    explicit GameState(const std::string& stateId);
    virtual ~GameState() = default;

    const std::string& getStateId() const;

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void handleInput() = 0;
    virtual void render() = 0; // 👈 Добавляем виртуальный метод render

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
};

#endif // GAMESTATE_H