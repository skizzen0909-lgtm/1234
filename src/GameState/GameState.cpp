// GameState/GameState.cpp

#include "GameState.h"
#include <stdexcept>

GameState::GameState(const std::string& stateId)
    : stateId(stateId) {
    if (stateId.empty()) {
        throw std::invalid_argument("State ID cannot be empty");
    }
}

const std::string& GameState::getStateId() const {
    return stateId;
}
