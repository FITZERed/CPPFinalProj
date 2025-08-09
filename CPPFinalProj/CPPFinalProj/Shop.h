#pragma once
#include "Inventory.h"
#define NOMINMAX // Avoid Windows min/max macros
#include <windows.h>
#include <iostream>

// Shop represents a merchant in the world.
// - name: display name
// - inventory: holds money and resource counts
// - mainResource: the resource this shop sells (set by template loader)
// - priorities[]: buying preference for each resource (0 to 4)
class Shop {
public:
    std::string name;
    Inventory inventory;
    ResourceType mainResource = ResourceType::Spices;
    int zone = -1;
    int id = -1;
    COORD position = { -1, -1 };
    int priorities[static_cast<int>(ResourceType::Count)]; // index = resource type

    int GetPriceForBuy();  // fixed price for main resource (not used yet)
    int GetPriceForSell(int resourceIndex); // price with multiplier (not used yet)

    bool CanBuy(int resourceIndex);
    bool CanSell();

    void Randomize(); // On map init (if needed)
};