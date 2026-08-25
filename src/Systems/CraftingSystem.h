#pragma once

#include <string>
#include <vector>
#include <map>
#include "Items/Item.h"

struct CraftingIngredient {
    std::string itemId;
    int quantity;
};

struct CraftingRecipe {
    std::string id;
    std::string name;
    std::string resultItemId;
    int resultQuantity;
    std::vector<CraftingIngredient> ingredients;
    
    bool canCraft(const std::map<std::string, int>& inventory) const {
        for (const auto& ingredient : ingredients) {
            auto it = inventory.find(ingredient.itemId);
            if (it == inventory.end() || it->second < ingredient.quantity) {
                return false;
            }
        }
        return true;
    }
};

class CraftingSystem {
public:
    CraftingSystem();
    
    void loadRecipes();
    bool craft(const std::string& recipeId, std::map<std::string, int>& inventory, std::vector<Item>& outputItems);
    const std::vector<CraftingRecipe>& getRecipes() const { return recipes; }
    
private:
    std::vector<CraftingRecipe> recipes;
    
    void addRecipe(const CraftingRecipe& recipe);
};
