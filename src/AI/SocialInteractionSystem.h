#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Простая структура для представления NPC
struct NPC {
    std::string id;
    std::string name;
    std::string role; // торговец, стражник, фермер и т.д.
    float relationship; // -1.0 до 1.0
    bool isAvailable;
};

struct DialogueOption {
    std::string text;
    int nextNode; // ID следующего узла или -1 для конца
    std::function<void()> action; // Действие при выборе
};

struct DialogueNode {
    int id;
    std::string speaker;
    std::string text;
    std::vector<DialogueOption> options;
};

/**
 * SocialInteractionSystem - Система социального взаимодействия между агентом и NPC.
 * Поддерживает диалоги, отношения и репутацию.
 */
class SocialInteractionSystem {
public:
    SocialInteractionSystem();
    ~SocialInteractionSystem();

    // Инициализация
    void initialize();

    // Регистрация NPC в системе
    void registerNPC(const NPC& npc);
    
    // Получение списка доступных NPC рядом с агентом
    std::vector<NPC> getNearbyNPCs(const std::string& agentLocation) const;

    // Начало диалога с NPC
    bool startDialogue(const std::string& npcId, const std::string& agentId);
    
    // Выбор опции в диалоге
    void selectDialogueOption(int optionIndex);
    
    // Завершение диалога
    void endDialogue();
    
    // Проверка активного диалога
    bool isDialogueActive() const;
    
    // Получение текущего узла диалога
    const DialogueNode* getCurrentDialogueNode() const;

    // Изменение отношений с NPC
    void modifyRelationship(const std::string& npcId, float delta);
    float getRelationship(const std::string& npcId) const;

    // Торговля с NPC (если роль позволяет)
    bool canTrade(const std::string& npcId) const;
    void initiateTrade(const std::string& npcId);

    // Обновление системы (для таймеров и событий)
    void update(float deltaTime);

private:
    std::unordered_map<std::string, NPC> npcs;
    std::unordered_map<std::string, std::vector<DialogueNode>> dialogues;
    
    std::string currentAgentId;
    std::string currentNPCId;
    int currentDialogueNodeIndex;
    bool dialogueActive;

    // Генерация базовых диалогов для NPC
    void generateDefaultDialogue(const std::string& npcId);
};
