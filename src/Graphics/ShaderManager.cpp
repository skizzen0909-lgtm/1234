// Graphics/ShaderManager.cpp
#include "ShaderManager.h"
#include <iostream>
#include <utility>
#include <memory>
bool ShaderManager::loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile) {
    auto shader = std::make_unique<sf::Shader>(); // ✅ Создаем уникальный указатель
    if (!shader->loadFromFile(vertexFile, fragmentFile)) {
        std::cerr << "Failed to load shader: " << name << std::endl;
        return false;
    }
    shaderCache.erase(name);
    shaderCache.emplace(name, std::move(shader)); // ✅ Перемещаем unique_ptr — это ОК!
    return true;
}

sf::Shader* ShaderManager::getShader(const std::string& name) {
    auto it = shaderCache.find(name);
    if (it != shaderCache.end()) {
        return it->second.get(); 
    }
    return nullptr;
}

void ShaderManager::clear() {
    shaderCache.clear();
}