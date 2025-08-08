#include "ShopManager.h"
#include "ShopTemplateLoader.h"
#include <iostream> // optional, for debug printing

void ShopManager::LoadShops() {
    // Load up to 5 shops from "ShopTemplates/shops.txt"
    // Note: loader takes (count, filePath)
    std::vector<Shop> templates = ShopTemplateLoader::LoadRandomTemplates(5, "ShopTemplates/shops.txt");

    shops.clear();
    shops.reserve(templates.size());
    for (size_t i = 0; i < templates.size(); ++i) {
        Shop shop = templates[i];
        // Ensure id is unique within this run (loader already assigned ids per template order,
        // but we can reassign to be safe)
        shop.id = static_cast<int>(i);
        shop.zone = -1; // will be set while placing on map
        shop.position = { -1, -1 };
        shops.push_back(shop);
    }
}

Shop* ShopManager::GetShopByID(int id) {
    for (auto& s : shops) {
        if (s.id == id) return &s;
    }
    return nullptr;
}