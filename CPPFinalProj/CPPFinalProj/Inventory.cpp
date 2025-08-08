// Inventory.cpp
#include "Inventory.h"
#include <algorithm> // for std::max/min (not strictly necessary here)
#include <cstring>   // for std::memset

Inventory::Inventory() {
    money = 0;
    // initialize all resource counts to 0
    for (int i = 0; i < static_cast<int>(ResourceType::Count); ++i) {
        resourceCounts[i] = 0;
    }
}

void Inventory::Add(ResourceType type, int amount) {
    if (amount <= 0) return; // ignore non-positive adds
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= static_cast<int>(ResourceType::Count)) return;
    resourceCounts[idx] += amount;
}

void Inventory::Remove(ResourceType type, int amount) {
    if (amount <= 0) return; // ignore non-positive removes
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= static_cast<int>(ResourceType::Count)) return;
    resourceCounts[idx] -= amount;
    if (resourceCounts[idx] < 0) resourceCounts[idx] = 0;
}

int Inventory::GetAmount(ResourceType type) const {
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= static_cast<int>(ResourceType::Count)) return 0;
    return resourceCounts[idx];
}