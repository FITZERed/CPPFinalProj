#include "ShopManager.h"
#include "ShopTemplateLoader.h"

void ShopManager::LoadShops() {
    auto templates = LoadRandomShopTemplates("ShopTemplates/shops.txt", 5);

    shops.clear();
    for (int i = 0; i < templates.size(); ++i) {
        const auto& temp = templates[i];

        Shop shop;
        shop.name = temp.name;
        shop.inventory = temp.stock;
        shop.zone = -1;  // set later, not now
        shop.id = temp.id;
        shop.position = { 0, 0 }; // set later in map loading
        for (int j = 0; j < 5; ++j) {
            shop.priorities[j] = temp.priorities[j];
        }

        shops.push_back(shop);
    }
}

Shop* ShopManager::GetShopByID(int id) {
    for (auto& shop : shops) {
        if (shop.id == id) return &shop;
    }
    return nullptr;
}