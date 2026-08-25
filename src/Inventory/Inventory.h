#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include <memory>

// Forward declarations
class Item;

class Inventory {
private:
    std::vector<std::shared_ptr<Item>> items;
    int maxCapacity;

public:
    // Конструкторы
    explicit Inventory(int maxCapacity = 20);
    ~Inventory() = default;

    // Запрет копирования
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    // Методы управления предметами
    bool addItem(std::shared_ptr<Item> item);
    bool removeItem(const std::shared_ptr<Item>& item);
    bool hasItem(const std::shared_ptr<Item>& item) const;
    void clear();

    // Геттеры
    const std::vector<std::shared_ptr<Item>>& getItems() const;
    int getItemCount() const;
    int getMaxCapacity() const;
    bool isFull() const;

    // Поиск предметов
    std::shared_ptr<Item> findItemByName(const std::string& name) const;
};

#endif // INVENTORY_H