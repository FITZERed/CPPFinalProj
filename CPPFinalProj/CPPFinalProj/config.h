#pragma once
constexpr int STARTING_MONEY = 300;
constexpr int STARTING_ACTION_POINTS = 15;
constexpr int WEEKLY_QUOTA = 500;
constexpr int WEEKLY_QUOTA_INCREMENT = 250;

constexpr int SHOP_MIN_MONEY = 100;
constexpr int SHOP_MAX_MONEY = 250;

constexpr int SHOP_MIN_RESOURCE = 3;
constexpr int SHOP_MAX_RESOURCE = 7;

// Priority multipliers: [0] = lowest, [4] = highest
constexpr float PRIORITY_MULTIPLIERS[5] = { 0.25f, 0.5f, 0.75f, 1.0f, 1.25f };

constexpr const char* RESOURCE_NAMES[5] = {
    "Spices", "Textiles", "Jewelry", "Minerals", "Medicine"


};
// --- Base prices for each resource ---
// Index MUST match ResourceType enum order and RESOURCE_NAMES array
constexpr int BASE_PRICES[5] = {
    4, // Spices
    4, // Textiles
    16, // Jewelry
    8, // Minerals
    12  // Medicine
};
constexpr int MAP_WIDTH = 62;
constexpr int MAP_HEIGHT = 20;

constexpr int GetZoneFromChar(char c) {
    switch (c) {
    case '.': return 0;
    case ',': return 1;
    case '~': return 2;
    case '\'': return 3;
    default: return -1;
    }
}