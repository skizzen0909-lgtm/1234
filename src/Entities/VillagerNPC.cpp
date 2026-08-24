// Entities/VillagerNPC.cpp
#include "VillagerNPC.h"
#include "Entities/Entity.h"
#include "Quests/Quest.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <memory>
VillagerNPC::VillagerNPC(const std::string& name, const std::string& occupation, int health)
    : NPC(name, "villager", health), occupation(occupation) {
}

std::string VillagerNPC::getType() const {
    return "villager";
}

void VillagerNPC::interact(Entity* player) {
    if (!player) return;
    NPC::interact(player);
    if (!offeredQuests.empty()) {
        std::cout << getName() << " предлагает квесты:" << std::endl;
        for (const auto& quest : offeredQuests) {
            if (quest) {
                std::cout << "- " << quest->getTitle() << std::endl;
            }
        }
    }
}

std::string VillagerNPC::getDialogue() const {
    if (occupation == "торговец") {
        return "Добро пожаловать в мой магазин!";
    }
    else if (occupation == "фермер") {
        return "Урожай в этом году отличный!";
    }
    else {
        return "Привет, путник! Что привело тебя в нашу деревню?";
    }
}

const std::string& VillagerNPC::getOccupation() const {
    return occupation;
}

const std::vector<std::shared_ptr<Quest>>& VillagerNPC::getOfferedQuests() const {
    return offeredQuests;
}

void VillagerNPC::setOccupation(const std::string& occupation) {
    this->occupation = occupation;
}

void VillagerNPC::addQuest(std::shared_ptr<Quest> quest) {
    if (quest) {
        offeredQuests.push_back(quest);
    }
}

void VillagerNPC::removeQuest(const std::shared_ptr<Quest>& quest) {
    if (!quest) return;
    auto it = std::find(offeredQuests.begin(), offeredQuests.end(), quest);
    if (it != offeredQuests.end()) {
        offeredQuests.erase(it);
    }
}

std::pair<int, int> VillagerNPC::decideMovement() {
    if (posX == -1 || posY == -1) return { -1, -1 };
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    // Плавное блуждание: с вероятностью 70% продолжаем в том же направлении
    // Для простоты, будем считать, что предыдущее направление хранится где-то (например, в виде поля)
    // Вместо этого, просто выбираем новое направление с некоторой "инерцией"
    int newDirX, newDirY;
    // 70% шанс сохранить прежнее направление (мы его не храним, поэтому делаем фиксированное)
    if (std::rand() % 100 < 70) {
        // Просто немного сместимся в сторону центра деревни
        newDirX = (posX < 25) ? 1 : (posX > 25) ? -1 : 0;
        newDirY = (posY < 25) ? 1 : (posY > 25) ? -1 : 0;
    }
    else {
        newDirX = dist(gen);
        newDirY = dist(gen);
    }

    int nx = posX + newDirX;
    int ny = posY + newDirY;

    // Проверка границ карты
    if (nx < 0 || ny < 0 || nx >= 50 || ny >= 50) {
        return { -1, -1 };
    }

    // Можно идти в любую сторону — случайное блуждание
    return { nx, ny };
}