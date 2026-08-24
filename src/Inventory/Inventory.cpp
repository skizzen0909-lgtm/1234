#include "Inventory.h"
#include "../Items/Item.h"
#include <algorithm>
#include <stdexcept>
#include <memory>
Inventory::Inventory(int maxCapacity) : maxCapacity(maxCapacity) {
    if (maxCapacity <= 0) {
        throw std::invalid_argument("Max capacity must be positive");
    }
}

bool Inventory::addItem(std::shared_ptr<Item> item) {
    if (!item) {
        return false;
    }

    if (isFull()) {
        return false;
    }

    items.push_back(item);
    return true;
}

bool Inventory::removeItem(const std::shared_ptr<Item>& item) {
    if (!item) {
        return false;
    }

    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        items.erase(it);
        return true;
    }
    return false;
}

bool Inventory::hasItem(const std::shared_ptr<Item>& item) const {
    return std::find(items.begin(), items.end(), item) != items.end();
}

void Inventory::clear() {
    items.clear();
}

const std::vector<std::shared_ptr<Item>>& Inventory::getItems() const {
    return items;
}

int Inventory::getItemCount() const {
    return static_cast<int>(items.size());
}

int Inventory::getMaxCapacity() const {
    return maxCapacity;
}

bool Inventory::isFull() const {
    return getItemCount() >= maxCapacity;
}

std::shared_ptr<Item> Inventory::findItemByName(const std::string& name) const {
    auto it = std::find_if(items.begin(), items.end(),
        [&name](const std::shared_ptr<Item>& item) {
            // Предполагаем, что у Item есть метод getName()
            // return item->getName() == name;
            return false; // Заглушка, так как Item.h не предоставлен
        });

    if (it != items.end()) {
        return *it;
    }
    return nullptr;
}