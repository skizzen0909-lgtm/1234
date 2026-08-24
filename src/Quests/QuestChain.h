// Quests/QuestChain.h
#ifndef QUESTCHAIN_H
#define QUESTCHAIN_H

#include <vector>
#include <memory>
#include <string>
#include "Quest.h"

class QuestChain {
private:
    std::string chainId;
    std::string chainName;
    std::string description;
    std::vector<std::shared_ptr<Quest>> quests;
    size_t currentQuestIndex;
    bool completed;
    std::string giverNPC; // NPC, который дал цепочку
    
public:
    // Конструктор
    QuestChain(const std::string& chainId, 
               const std::string& chainName,
               const std::string& description,
               const std::string& giverNPC = "");
    
    ~QuestChain() = default;
    
    // Добавление квеста в цепочку
    void addQuest(std::shared_ptr<Quest> quest);
    
    // Получение текущего квеста
    std::shared_ptr<Quest> getCurrentQuest() const;
    
    // Получение следующего квеста
    std::shared_ptr<Quest> getNextQuest() const;
    
    // Завершение текущего квеста и переход к следующему
    bool completeCurrentQuest();
    
    // Проверка завершения всей цепочки
    bool isChainCompleted() const;
    
    // Прогресс цепочки
    size_t getCurrentQuestIndex() const;
    size_t getTotalQuests() const;
    float getProgressPercentage() const;
    
    // Геттеры
    const std::string& getChainId() const;
    const std::string& getChainName() const;
    const std::string& getDescription() const;
    const std::string& getGiverNPC() const;
    bool isCompleted() const;
    
    // Сеттеры
    void setGiverNPC(const std::string& npc);
    
    // Статус цепочки
    std::string getChainStatus() const;
    
    // Награды за завершение цепочки
    virtual void giveChainRewards(class Entity* player);
    
    // Запрет копирования
    QuestChain(const QuestChain&) = delete;
    QuestChain& operator=(const QuestChain&) = delete;
};

#endif // QUESTCHAIN_H
