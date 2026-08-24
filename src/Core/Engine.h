// Core/Engine.h
#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <stack>
#include <string>

//class GraphicsSystem;
class AdvancedGraphicsSystem;
class GameState;

class Engine {
private:
    static Engine* instance;
    std::unique_ptr<AdvancedGraphicsSystem> graphicsSystem;
    std::stack<std::unique_ptr<GameState>> stateStack;
    bool running;
    float timeScale;

    Engine(); // Private constructor for singleton

public:
    ~Engine();

    static Engine* getInstance();
    static void destroyInstance();

    bool initialize();
    void shutdown();
    void run();

    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    GameState* getCurrentState() const;

    bool isRunning() const;
    void stop();
    void setTimeScale(float scale);
    float getTimeScale() const;

    AdvancedGraphicsSystem* getGraphicsSystem() const; // Добавлен метод
};

#endif