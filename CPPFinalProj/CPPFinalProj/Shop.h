#pragma once
#include "Inventory.h"
#define NOMINMAX   // Prevent Windows min/max macros from messing with std::min/std::max
#include <windows.h>
#include <iostream>

class Shop {
public:
    std::string name;
    Inventory inventory;
    int zone;
    int id;
    COORD position;
    int priorities[5]; // index = resource type, 1–4 priority

    int GetPriceForBuy();  // fixed price for main resource
    int GetPriceForSell(int resourceIndex); // price with multiplier

    bool CanBuy(int resourceIndex);
    bool CanSell();

    void Randomize(); // On map init
};