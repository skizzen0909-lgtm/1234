// Entities/NPC.cpp

#include "NPC.h"
#include "Entities/Entity.h"
#include <iostream>


NPC::NPC(const std::string& name, const std::string& npcType, int health)
    : Entity(name, health), npcType(npcType), relationshipLevel(0) {
}

std::string NPC::getType() const {
    return "npc";
}

const std::string& NPC::getNpcType() const {
    return npcType;
}

int NPC::getRelationshipLevel() const {
    return relationshipLevel;
}

void NPC::setNpcType(const std::string& type) {
    npcType = type;
}

void NPC::setRelationshipLevel(int level) {
    relationshipLevel = level;
}

void NPC::interact(Entity* player) {
    if (!player) return;

    std::cout << getName() << " говорит: " << getDialogue() << std::endl;
}

std::string NPC::getDialogue() const {
    return "Привет, путник!";
}