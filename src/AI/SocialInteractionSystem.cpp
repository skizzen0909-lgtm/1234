#include "SocialInteractionSystem.h"
#include <iostream>
#include <algorithm>

SocialInteractionSystem::SocialInteractionSystem() 
    : currentDialogueNodeIndex(0), dialogueActive(false) {
}

SocialInteractionSystem::~SocialInteractionSystem() {
}

void SocialInteractionSystem::initialize() {
    std::cout << "[SocialInteractionSystem] Инициализация системы социального взаимодействия." << std::endl;
    
    // Пример регистрации NPC (в реальной игре это будет загружаться из мира)
    NPC merchant;
    merchant.id = "npc_001";
    merchant.name = "Торговец Джон";
    merchant.role = "merchant";
    merchant.relationship = 0.0f;
    merchant.isAvailable = true;
    registerNPC(merchant);
    
    NPC guard;
    guard.id = "npc_002";
    guard.name = "Страж Анна";
    guard.role = "guard";
    guard.relationship = 0.2f;
    guard.isAvailable = true;
    registerNPC(guard);
    
    NPC farmer;
    farmer.id = "npc_003";
    farmer.name = "Фермер Боб";
    farmer.role = "farmer";
    farmer.relationship = 0.1f;
    farmer.isAvailable = true;
    registerNPC(farmer);
}

void SocialInteractionSystem::registerNPC(const NPC& npc) {
    npcs[npc.id] = npc;
    generateDefaultDialogue(npc.id);
    std::cout << "[SocialInteractionSystem] Зарегистрирован NPC: " << npc.name << std::endl;
}

std::vector<NPC> SocialInteractionSystem::getNearbyNPCs(const std::string& agentLocation) const {
    std::vector<NPC> nearby;
    // В упрощенной версии возвращаем всех доступных NPC
    // В полной игре здесь будет проверка координат
    for (const auto& pair : npcs) {
        if (pair.second.isAvailable) {
            nearby.push_back(pair.second);
        }
    }
    return nearby;
}

bool SocialInteractionSystem::startDialogue(const std::string& npcId, const std::string& agentId) {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) {
        std::cout << "[SocialInteractionSystem] NPC не найден: " << npcId << std::endl;
        return false;
    }
    
    if (!it->second.isAvailable) {
        std::cout << "[SocialInteractionSystem] NPC недоступен для диалога: " << npcId << std::endl;
        return false;
    }
    
    currentNPCId = npcId;
    currentAgentId = agentId;
    currentDialogueNodeIndex = 0;
    dialogueActive = true;
    
    std::cout << "[SocialInteractionSystem] Начало диалога с " << it->second.name << std::endl;
    return true;
}

void SocialInteractionSystem::selectDialogueOption(int optionIndex) {
    if (!dialogueActive) {
        std::cout << "[SocialInteractionSystem] Диалог не активен." << std::endl;
        return;
    }
    
    auto it = dialogues.find(currentNPCId);
    if (it == dialogues.end() || currentDialogueNodeIndex >= static_cast<int>(it->second.size())) {
        std::cout << "[SocialInteractionSystem] Неверный узел диалога." << std::endl;
        return;
    }
    
    DialogueNode& currentNode = it->second[currentDialogueNodeIndex];
    
    if (optionIndex < 0 || optionIndex >= static_cast<int>(currentNode.options.size())) {
        std::cout << "[SocialInteractionSystem] Неверная опция диалога." << std::endl;
        return;
    }
    
    // Выполнение действия опции
    if (currentNode.options[optionIndex].action) {
        currentNode.options[optionIndex].action();
    }
    
    // Переход к следующему узлу
    int nextNode = currentNode.options[optionIndex].nextNode;
    if (nextNode == -1) {
        endDialogue();
    } else {
        currentDialogueNodeIndex = nextNode;
        const DialogueNode* newNode = getCurrentDialogueNode();
        if (newNode) {
            std::cout << "[" << newNode->speaker << "]: " << newNode->text << std::endl;
        }
    }
}

void SocialInteractionSystem::endDialogue() {
    if (!dialogueActive) return;
    
    std::cout << "[SocialInteractionSystem] Диалог завершен." << std::endl;
    dialogueActive = false;
    currentNPCId = "";
    currentAgentId = "";
    currentDialogueNodeIndex = 0;
}

bool SocialInteractionSystem::isDialogueActive() const {
    return dialogueActive;
}

const DialogueNode* SocialInteractionSystem::getCurrentDialogueNode() const {
    if (!dialogueActive) return nullptr;
    
    auto it = dialogues.find(currentNPCId);
    if (it == dialogues.end() || currentDialogueNodeIndex >= static_cast<int>(it->second.size())) {
        return nullptr;
    }
    
    return &it->second[currentDialogueNodeIndex];
}

void SocialInteractionSystem::modifyRelationship(const std::string& npcId, float delta) {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) return;
    
    it->second.relationship += delta;
    // Ограничение диапазона [-1.0, 1.0]
    it->second.relationship = std::max(-1.0f, std::min(1.0f, it->second.relationship));
    
    std::cout << "[SocialInteractionSystem] Отношения с " << it->second.name 
              << " изменены на " << delta << " (текущее: " << it->second.relationship << ")" << std::endl;
}

float SocialInteractionSystem::getRelationship(const std::string& npcId) const {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) return 0.0f;
    return it->second.relationship;
}

bool SocialInteractionSystem::canTrade(const std::string& npcId) const {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) return false;
    return it->second.role == "merchant" || it->second.role == "trader";
}

void SocialInteractionSystem::initiateTrade(const std::string& npcId) {
    if (!canTrade(npcId)) {
        std::cout << "[SocialInteractionSystem] NPC " << npcId << " не может торговать." << std::endl;
        return;
    }
    
    std::cout << "[SocialInteractionSystem] Начало торговли с " << npcId << std::endl;
    // Здесь будет логика открытия интерфейса торговли
}

void SocialInteractionSystem::update(float deltaTime) {
    // Обновление таймеров, восстановление доступности NPC и т.д.
    // В полной версии здесь будет обработка временных событий
}

void SocialInteractionSystem::generateDefaultDialogue(const std::string& npcId) {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) return;
    
    std::vector<DialogueNode> dialogueTree;
    
    // Узел 0: Приветствие
    DialogueNode greeting;
    greeting.id = 0;
    greeting.speaker = it->second.name;
    greeting.text = "Приветствую тебя, путник! Чем могу помочь?";
    
    // Опции для узла 0
    DialogueOption opt1;
    opt1.text = "Расскажи о себе";
    opt1.nextNode = 1;
    opt1.action = []() { /* Действие при выборе */ };
    
    DialogueOption opt2;
    opt2.text = "Есть ли новости?";
    opt2.nextNode = 2;
    opt2.action = []() { /* Действие при выборе */ };
    
    DialogueOption opt3;
    opt3.text = "До свидания";
    opt3.nextNode = -1;
    opt3.action = []() { /* Действие при выборе */ };
    
    greeting.options = {opt1, opt2, opt3};
    dialogueTree.push_back(greeting);
    
    // Узел 1: О себе
    DialogueNode about;
    about.id = 1;
    about.speaker = it->second.name;
    about.text = "Я " + it->second.name + ", работаю как " + it->second.role + ". Живу здесь уже много лет.";
    
    DialogueOption back1;
    back1.text = "Вернуться";
    back1.nextNode = 0;
    back1.action = []() {};
    
    about.options = {back1};
    dialogueTree.push_back(about);
    
    // Узел 2: Новости
    DialogueNode news;
    news.id = 2;
    news.speaker = it->second.name;
    news.text = "Ходят слухи о странных событиях в ближайших лесах. Будь осторожен!";
    
    DialogueOption back2;
    back2.text = "Спасибо за предупреждение";
    back2.nextNode = 0;
    back2.action = [this, npcId]() { 
        modifyRelationship(npcId, 0.05f); // Небольшое улучшение отношений за благодарность
    };
    
    news.options = {back2};
    dialogueTree.push_back(news);
    
    dialogues[npcId] = dialogueTree;
}
