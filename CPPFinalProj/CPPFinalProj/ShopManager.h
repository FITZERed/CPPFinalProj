#pragma once
#include "Shop.h"
#include "ShopTemplateLoader.h"
#include <vector>

class ShopManager {
private:
    std::vector<Shop> shops; // List of shops created from templates

public:
    void LoadShops(); // loads shops from file
    Shop* GetShopByID(int id); // retrieve shop by ID
};