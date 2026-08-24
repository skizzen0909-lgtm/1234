// main.cpp
#include "Core/Engine.h"
#include "GameState/EnhancedPlayState.h"
#include "World/AdvancedWorldGenerator.h"
#include <iostream>
#include <memory>
#include <locale>

int main() {
    std::setlocale(LC_ALL, "");
    
    try {
        std::cout << "=== Eternal Wanderer - Enhanced AFK RPG ===" << std::endl;

        // Используем getInstance() вместо прямого создания
        Engine* gameEnginePtr = Engine::getInstance();
        if (!gameEnginePtr) {
            std::cerr << "Failed to get game engine instance!" << std::endl;
            return -1;
        }

        if (!gameEnginePtr->initialize()) {
            std::cerr << "Failed to initialize game engine!" << std::endl;
            Engine::destroyInstance(); // Не забываем освободить память
            return -1;
        }

        auto playState = std::make_unique<EnhancedPlayState>();
        gameEnginePtr->changeState(std::move(playState));
        gameEnginePtr->run();
        gameEnginePtr->shutdown();
        Engine::destroyInstance(); // Освобождаем синглтон

    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        Engine::destroyInstance(); // На всякий случай
        return -1;
    }

    return 0;
}