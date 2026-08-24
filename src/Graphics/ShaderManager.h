// Graphics/ShaderManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ShaderManager {
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaderCache;
public:
    bool loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);
    sf::Shader* getShader(const std::string& name);
    void clear();
};