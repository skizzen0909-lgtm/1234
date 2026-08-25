#include "CraftingSystem.h"
#include <iostream>

CraftingSystem::CraftingSystem() {
    loadRecipes();
}

void CraftingSystem::loadRecipes() {
    // Рецепт: Зелье здоровья
    CraftingRecipe healthPotion;
    healthPotion.id = "health_potion";
    healthPotion.name = "Зелье здоровья";
    healthPotion.resultItemId = "item_health_potion";
    healthPotion.resultQuantity = 1;
    healthPotion.ingredients.push_back({"herb_green", 3});
    healthPotion.ingredients.push_back({"water_pure", 1});
    addRecipe(healthPotion);
    
    // Рецепт: Меч из стали
    CraftingRecipe steelSword;
    steelSword.id = "steel_sword";
    steelSword.name = "Стальной меч";
    steelSword.resultItemId = "weapon_steel_sword";
    steelSword.resultQuantity = 1;
    steelSword.ingredients.push_back({"iron_ingot", 5});
    steelSword.ingredients.push_back({"wood_oak", 2});
    addRecipe(steelSword);
    
    // Рецепт: Деревянный щит
    CraftingRecipe woodenShield;
    woodenShield.id = "wooden_shield";
    woodenShield.name = "Деревянный щит";
    woodenShield.resultItemId = "armor_wooden_shield";
    woodenShield.resultQuantity = 1;
    woodenShield.ingredients.push_back({"wood_oak", 4});
    woodenShield.ingredients.push_back({"leather", 2});
    addRecipe(woodenShield);
    
    // Рецепт: Зелье маны
    CraftingRecipe manaPotion;
    manaPotion.id = "mana_potion";
    manaPotion.name = "Зелье маны";
    manaPotion.resultItemId = "item_mana_potion";
    manaPotion.resultQuantity = 1;
    manaPotion.ingredients.push_back({"herb_blue", 3});
    manaPotion.ingredients.push_back({"water_pure", 1});
    addRecipe(manaPotion);
    
    // Рецепт: Железная броня
    CraftingRecipe ironArmor;
    ironArmor.id = "iron_armor";
    ironArmor.name = "Железная броня";
    ironArmor.resultItemId = "armor_iron_chestplate";
    ironArmor.resultQuantity = 1;
    ironArmor.ingredients.push_back({"iron_ingot", 10});
    ironArmor.ingredients.push_back({"leather", 5});
    addRecipe(ironArmor);
    
    std::cout << "Загружено " << recipes.size() << " рецептов крафта." << std::endl;
}

void CraftingSystem::addRecipe(const CraftingRecipe& recipe) {
    recipes.push_back(recipe);
}

bool CraftingSystem::craft(const std::string& recipeId, std::map<std::string, int>& inventory, std::vector<Item>& outputItems) {
    // Поиск рецепта
    CraftingRecipe* selectedRecipe = nullptr;
    for (auto& recipe : recipes) {
        if (recipe.id == recipeId) {
            selectedRecipe = &recipe;
            break;
        }
    }
    
    if (!selectedRecipe) {
        std::cerr << "Рецепт не найден: " << recipeId << std::endl;
        return false;
    }
    
    // Проверка возможности крафта
    if (!selectedRecipe->canCraft(inventory)) {
        std::cerr << "Недостаточно ресурсов для крафта: " << selectedRecipe->name << std::endl;
        return false;
    }
    
    // Списание ингредиентов
    for (const auto& ingredient : selectedRecipe->ingredients) {
        inventory[ingredient.itemId] -= ingredient.quantity;
        if (inventory[ingredient.itemId] <= 0) {
            inventory.erase(ingredient.itemId);
        }
    }
    
    // Создание результата
    // Примечание: здесь должна быть логика создания Item объекта
    // Для простоты пока просто выводим сообщение
    std::cout << "Скрафчено: " << selectedRecipe->name 
              << " x" << selectedRecipe->resultQuantity << std::endl;
    
    return true;
}
