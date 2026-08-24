// Quests/QuestChain.cpp
#include "QuestChain.h"
#include "Entities/Entity.h"
#include <sstream>

QuestChain::QuestChain(const std::string& chainId,
                       const std::string& chainName,
                       const std::string& description,
                       const std::string& giverNPC)
    : chainId(chainId),
      chainName(chainName),
      description(description),
      currentQuestIndex(0),
      completed(false),
      giverNPC(giverNPC) {
}

void QuestChain::addQuest(std::shared_ptr<Quest> quest) {
    if (quest) {
        quests.push_back(quest);
    }
}

std::shared_ptr<Quest> QuestChain::getCurrentQuest() const {
    if (currentQuestIndex < quests.size()) {
        return quests[currentQuestIndex];
    }
    return nullptr;
}

std::shared_ptr<Quest> QuestChain::getNextQuest() const {
    if (currentQuestIndex + 1 < quests.size()) {
        return quests[currentQuestIndex + 1];
    }
    return nullptr;
}

bool QuestChain::completeCurrentQuest() {
    auto currentQuest = getCurrentQuest();
    if (!currentQuest || !currentQuest->isCompleted()) {
        return false;
    }
    
    currentQuestIndex++;
    
    // Проверка завершения всей цепочки
    if (currentQuestIndex >= quests.size()) {
        completed = true;
    }
    
    return true;
}

bool QuestChain::isChainCompleted() const {
    return completed;
}

size_t QuestChain::getCurrentQuestIndex() const {
    return currentQuestIndex;
}

size_t QuestChain::getTotalQuests() const {
    return quests.size();
}

float QuestChain::getProgressPercentage() const {
    if (quests.empty()) {
        return 0.0f;
    }
    return (static_cast<float>(currentQuestIndex) / quests.size()) * 100.0f;
}

const std::string& QuestChain::getChainId() const {
    return chainId;
}

const std::string& QuestChain::getChainName() const {
    return chainName;
}

const std::string& QuestChain::getDescription() const {
    return description;
}

const std::string& QuestChain::getGiverNPC() const {
    return giverNPC;
}

bool QuestChain::isCompleted() const {
    return completed;
}

void QuestChain::setGiverNPC(const std::string& npc) {
    giverNPC = npc;
}

std::string QuestChain::getChainStatus() const {
    std::ostringstream ss;
    ss << "Цепочка: " << chainName << "\n";
    ss << "Прогресс: " << currentQuestIndex << "/" << quests.size() << "\n";
    ss << "Статус: " << (completed ? "Завершено" : "Активно") << "\n";
    
    if (currentQuestIndex < quests.size()) {
        ss << "Текущий квест: " << quests[currentQuestIndex]->getTitle();
    }
    
    return ss.str();
}

void QuestChain::giveChainRewards(Entity* player) {
    if (!completed || !player) {
        return;
    }
    
    // Базовая реализация - дает бонусный опыт за завершение цепочки
    int bonusExperience = static_cast<int>(quests.size()) * 50;
    player->gainExperience(bonusExperience);
    
    // Можно расширить для выдачи уникальных наград
}
