#include "Item.h"
#include "Entities/Entity.h"
#include <iostream>
#include <memory>
Item::Item(const std::string& id, const std::string& name, const std::string& description,
           ItemType type, ItemRarity rarity, int value, int weight)
    : id(id), name(name), description(description), type(type), 
      rarity(rarity), value(value), weight(weight) {
    if (id.empty()) {
        throw std::invalid_argument("Item ID cannot be empty");
    }
    if (value < 0) {
        throw std::invalid_argument("Item value cannot be negative");
    }
    if (weight < 0) {
        throw std::invalid_argument("Item weight cannot be negative");
    }
}

const std::string& Item::getId() const {
    return id;
}

const std::string& Item::getName() const {
    return name;
}

const std::string& Item::getDescription() const {
    return description;
}

ItemType Item::getType() const {
    return type;
}

ItemRarity Item::getRarity() const {
    return rarity;
}

int Item::getValue() const {
    return value;
}

int Item::getWeight() const {
    return weight;
}

void Item::setName(const std::string& name) {
    this->name = name;
}

void Item::setDescription(const std::string& description) {
    this->description = description;
}

void Item::setType(ItemType type) {
    this->type = type;
}

void Item::setRarity(ItemRarity rarity) {
    this->rarity = rarity;
}

void Item::setValue(int value) {
    if (value >= 0) {
        this->value = value;
    }
}

void Item::setWeight(int weight) {
    if (weight >= 0) {
        this->weight = weight;
    }
}

void Item::use(Entity* user) {
    if (!user) return;
    
    std::cout << user->getName() << " использует " << name << std::endl;
    // Базовая реализация - ничего не делает
}

std::string Item::getTooltip() const {
    std::string rarityStr;
    switch (rarity) {
        case ItemRarity::COMMON: rarityStr = "Обычный"; break;
        case ItemRarity::UNCOMMON: rarityStr = "Необычный"; break;
        case ItemRarity::RARE: rarityStr = "Редкий"; break;
        case ItemRarity::EPIC: rarityStr = "Эпический"; break;
        case ItemRarity::LEGENDARY: rarityStr = "Легендарный"; break;
    }
    
    return name + " (" + rarityStr + ")\n" + description + 
           "\nЦена: " + std::to_string(value) + " монет" +
           "\nВес: " + std::to_string(weight);
}

std::unique_ptr<Item> Item::clone() const {
    return std::make_unique<Item>(id, name, description, type, rarity, value, weight);
}