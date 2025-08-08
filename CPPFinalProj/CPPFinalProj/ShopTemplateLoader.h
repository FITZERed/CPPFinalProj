#pragma once

#include "Shop.h"
#include <vector>
#include <string>

// Loads up to 'count' shop templates from a text file (filePath)
// The function returns at most 'count' shops (no duplicates).
// Template format (per-shop block, separated by a line with '---'):
// {Shop Name}
// ResourceSold: {ResourceName}
// Gold: {integer}
// Stock: {integer}
// Priorities: p_spices p_textiles p_jewelry p_minerals p_medicine
// ---
// Note: filePath is a relative path (e.g. "ShopTemplates/shops.txt")
class ShopTemplateLoader {
public:
    static std::vector<Shop> LoadRandomTemplates(int count, const std::string& filePath);
};