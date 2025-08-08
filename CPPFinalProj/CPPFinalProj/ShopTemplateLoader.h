#pragma once

#include "Shop.h"
#include <vector>
#include <string>

class ShopTemplateLoader {
public:
    static std::vector<Shop> LoadRandomTemplates(int count, const std::string& filePath);
};