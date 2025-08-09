#include "ShopTemplateLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <iostream>
#include <cctype> // For std::isspace
#include <algorithm> // For std::shuffle and std::min
#include "ResourceUtils.h"
#include "StringUtils.h" // We'll define this below
#include "Inventory.h"
#include "config.h"



// LoadRandomTemplates(count, filePath)
// - reads file, splits into blocks
// - parses blocks into Shop objects
// - shuffles and returns up to 'count' shops (no duplicates)
std::vector<Shop> ShopTemplateLoader::LoadRandomTemplates(int count, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ShopTemplateLoader: failed to open " << filePath << "\n";
        return {};
    }

    // 1) Read file and separate to blocks by '---'
    std::vector<std::string> blocks;
    std::string line;
    std::string block;
    while (std::getline(file, line)) {
        std::string t = Trim(line);
        if (t == "---") {
            if (!Trim(block).empty()) {
                blocks.push_back(block);
                block.clear();
            }
        }
        else {
            block += line;
            block.push_back('\n');
        }
    }
    if (!Trim(block).empty()) {
        blocks.push_back(block);
        block.clear();
    }

    // 2) Parse each block according to format:
    // Name (line 0)
    // ResourceSold: ResourceName
    // Gold: <int>
    // Stock: <int>
    // Priorities: p1 p2 p3 p4 p5
    std::vector<Shop> parsed;
    parsed.reserve(blocks.size());

    int nextId = 0;
    for (const std::string& b : blocks) {
        std::istringstream ss(b);
        std::string raw;
        std::vector<std::string> lines;
        while (std::getline(ss, raw)) {
            std::string t = Trim(raw);
            if (!t.empty()) lines.push_back(t);
        }

        if (lines.size() < 5) {
            std::cerr << "ShopTemplateLoader: skipping invalid template:\n" << b << "\n";
            continue;
        }

        Shop shop;
        shop.id = nextId++;
        shop.zone = -1;
        shop.position = { -1, -1 };
        for (int i = 0; i < static_cast<int>(ResourceType::Count); ++i) shop.priorities[i] = 0;

        // Name
        shop.name = lines[0];

        // parse keyed lines
        ResourceType parsedMain = ResourceType::Spices; // Defensive: ensure mainResource is set. If not, leave default (Spices).
        bool hasMain = false;

        for (size_t i = 1; i < lines.size(); ++i) {
            const std::string& ln = lines[i];
            size_t colon = ln.find(':');
            if (colon == std::string::npos) continue;
            std::string key = Trim(ln.substr(0, colon));
            std::string value = Trim(ln.substr(colon + 1));

            if (key == "ResourceSold") {
                parsedMain = StringToResource(value);
                hasMain = true;
                shop.mainResource = parsedMain;
            }
            else if (key == "Gold") {
                try {
                    shop.inventory.money = std::stoi(value);
                }
                catch (...) {
                    shop.inventory.money = 0;
                    std::cerr << "ShopTemplateLoader: invalid money value '" << value << "' for shop: " << shop.name << "\n";
                }
            }
            else if (key == "Stock") {
                try {
                    int stock = std::stoi(value);
                    if (hasMain) {
                        shop.inventory.Add(parsedMain, stock);
                    }
                    else {
                        // If Stock occurs before ResourceSold (unexpected), warn and skip
                        std::cerr << "ShopTemplateLoader: Stock found before ResourceSold in '" << shop.name << "'. Stock ignored.\n";
                    }
                }
                catch (...) {
                    std::cerr << "ShopTemplateLoader: invalid Stock value '" << value << "' for shop: " << shop.name << "\n";
                }
            }
            else if (key == "Priorities") {
                std::istringstream pis(value);
                for (int p = 0; p < static_cast<int>(ResourceType::Count); ++p) {
                    int pri = 0;
                    if (!(pis >> pri)) pri = 0;
                    // Clamp to 0..4
                    if (pri < 0) pri = 0;
                    if (pri > 4) pri = 4;
                    shop.priorities[p] = pri;
                }
            }
            else {
                std::cerr << "ShopTemplateLoader: unknown key '" << key << "' in template for shop: " << shop.name << "\n";
            }
        }
        parsed.push_back(shop);
    }

    // 3) Shuffle and pick up to 'count'
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(parsed.begin(), parsed.end(), gen);

    int take = std::min(count, static_cast<int>(parsed.size()));
    return std::vector<Shop>(parsed.begin(), parsed.begin() + take);
}