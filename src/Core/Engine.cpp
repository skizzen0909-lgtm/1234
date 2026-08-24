// Core/Engine.cpp
#include "Core/Engine.h"
#include "Graphics/AdvancedGraphicsSystem.h"
#include "GameState/EnhancedPlayState.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

// Инициализация статического члена
Engine* Engine::instance = nullptr;

// Конструктор теперь private
Engine::Engine() : running(false), timeScale(1.0f) {
    instance = this; // Устанавливаем глобальный указатель
}

Engine::~Engine() {
    shutdown();
}

// Реализация методов синглтона
Engine* Engine::getInstance() {
    if (!instance) {
        instance = new Engine(); // Создаем, если не существует
    }
    return instance;
}

void Engine::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

bool Engine::initialize() {
    graphicsSystem = std::make_unique<AdvancedGraphicsSystem>();
    if (!graphicsSystem->initialize()) {
        std::cerr << "Failed to initialize graphics system!" << std::endl;
        return false;
    }
    running = true;
    return true;
}

void Engine::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    const float targetFPS = 60.0f;
    const std::chrono::duration<float> targetFrameTime(1.0f / targetFPS);

    while (running && !stateStack.empty() && graphicsSystem->isWindowOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        deltaTime *= timeScale;
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        // --- ИСПРАВЛЕНИЕ: Обработка событий SFML напрямую ---
        sf::Event event;
        // Используем ссылку на окно из graphicsSystem для получения событий
        while (graphicsSystem->getWindow().pollEvent(event)) {
            // Проверяем закрытие окна
            if (event.type == sf::Event::Closed) {
                graphicsSystem->getWindow().close();
                running = false; // Останавливаем движок
            }
            // Передаем событие текущему состоянию, если нужно
            // if (GameState* currentState = getCurrentState()) {
            //     currentState->handleEvent(event); // Предполагая, что такой метод есть
            // }
        }
        // --- КОНЕЦ ИСПРАВЛЕНИЯ ---

        if (GameState* currentState = getCurrentState()) {
            currentState->handleInput();
            currentState->update(deltaTime);

            // --- ЦИКЛ РЕНДЕРИНГА ---
            std::cout << "Beginning frame..." << std::endl;
            graphicsSystem->beginFrame();

            std::cout << "Rendering state..." << std::endl;
            currentState->render();

            std::cout << "Ending frame..." << std::endl;
            graphicsSystem->endFrame();
            // --- КОНЕЦ ЦИКЛА РЕНДЕРИНГА ---
        }

        // Ограничение FPS
        auto endTime = std::chrono::high_resolution_clock::now();
        auto sleepTime = targetFrameTime - (endTime - currentTime);
        if (sleepTime > std::chrono::duration<float>::zero()) {
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

void Engine::shutdown() {
    while (!stateStack.empty()) {
        stateStack.top()->exit();
        stateStack.pop();
    }
    if (graphicsSystem) {
        graphicsSystem->shutdown();
    }
    running = false;
}

void Engine::pushState(std::unique_ptr<GameState> state) {
    if (state) {
        if (!stateStack.empty()) {
            stateStack.top()->exit();
        }
        stateStack.push(std::move(state));
        stateStack.top()->enter();
    }
}

void Engine::popState() {
    if (!stateStack.empty()) {
        stateStack.top()->exit();
        stateStack.pop();
        if (!stateStack.empty()) {
            stateStack.top()->enter();
        }
    }
}

void Engine::changeState(std::unique_ptr<GameState> state) {
    if (state) {
        while (!stateStack.empty()) {
            stateStack.top()->exit();
            stateStack.pop();
        }
        stateStack.push(std::move(state));
        stateStack.top()->enter();
    }
}

GameState* Engine::getCurrentState() const {
    return stateStack.empty() ? nullptr : stateStack.top().get();
}

bool Engine::isRunning() const {
    return running;
}

void Engine::stop() {
    running = false;
}

void Engine::setTimeScale(float scale) {
    timeScale = std::clamp(scale, 0.1f, 10.0f);
}

float Engine::getTimeScale() const {
    return timeScale;
}

AdvancedGraphicsSystem* Engine::getGraphicsSystem() const {
    return graphicsSystem.get();
}
