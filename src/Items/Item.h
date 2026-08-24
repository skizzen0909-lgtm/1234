#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>

enum class ItemType {
    WEAPON,
    ARMOR,
    CONSUMABLE,
    QUEST_ITEM,
    MISC
};

enum class ItemRarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

class Item {
protected:
    std::string id;
    std::string name;
    std::string description;
    ItemType type;
    ItemRarity rarity;
    int value; // Значение в монетах
    int weight; // Вес предмета

public:
    // Конструктор
    Item(const std::string& id, const std::string& name, const std::string& description,
         ItemType type, ItemRarity rarity = ItemRarity::COMMON, int value = 0, int weight = 1);
    
    // Виртуальный деструктор
    virtual ~Item() = default;
    
    // Геттеры
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    ItemType getType() const;
    ItemRarity getRarity() const;
    int getValue() const;
    int getWeight() const;
    
    // Сеттеры
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setType(ItemType type);
    void setRarity(ItemRarity rarity);
    void setValue(int value);
    void setWeight(int weight);
    
    // Виртуальные методы
    virtual void use(class Entity* user);
    virtual std::string getTooltip() const;
    virtual std::unique_ptr<Item> clone() const;
    
    // Запрет копирования
    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;
};

#endif // ITEM_H