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


// ---------------------
// Implementation for ShopTemplateLoader
// ---------------------
std::vector<Shop> ShopTemplateLoader::LoadRandomTemplates(int count, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ShopTemplateLoader: failed to open template file: " << filePath << "\n";
        return {};
    }

    // 1) Read file into blocks separated by a line that is exactly '---' (after trimming)
    std::vector<std::string> blocks;
    std::string currentLine;
    std::string currentBlock;
    while (std::getline(file, currentLine)) {
        std::string t = Trim(currentLine);
        if (t == "---") {
            if (!Trim(currentBlock).empty()) {
                blocks.push_back(currentBlock);
                currentBlock.clear();
            }
        }
        else {
            currentBlock += currentLine;
            currentBlock.push_back('\n');
        }
    }
    if (!Trim(currentBlock).empty()) {
        blocks.push_back(currentBlock);
        currentBlock.clear();
    }

    // 2) Parse each block into a Shop object
    std::vector<Shop> parsed;
    parsed.reserve(blocks.size());

    int nextId = 0;
    for (const std::string& block : blocks) {
        std::istringstream ss(block);
        std::string raw;
        std::vector<std::string> lines;

        // Collect non-empty trimmed lines (keeps order)
        while (std::getline(ss, raw)) {
            std::string t = Trim(raw);
            if (!t.empty()) lines.push_back(t);
        }

        // Defensive: need at least 5 lines (name, resource, gold, stock, priorities)
        if (lines.size() < 5) {
            // If the block is malformed, skip it but print a warning.
            std::cerr << "ShopTemplateLoader: skipping malformed template (expected >=5 lines):\n"
                << block << "\n";
            continue;
        }

        Shop shop;
        shop.id = nextId++;
        shop.zone = -1;
        shop.position = { -1, -1 };
        for (int i = 0; i < static_cast<int>(ResourceType::Count); ++i) shop.priorities[i] = 0;
        shop.name = lines[0]; // first line is the shop name

        // Parse the remaining lines by key:value
        // Expected keys: ResourceSold, Gold, Stock, Priorities
        // We'll parse them in any order for robustness, but we require them to exist.
        ResourceType mainResource = ResourceType::Spices; // default fallback
        bool hasResource = false;

        for (size_t li = 1; li < lines.size(); ++li) {
            const std::string& ln = lines[li];
            size_t colon = ln.find(':');
            if (colon == std::string::npos) {
                // Unexpected line format — skip
                continue;
            }
            std::string key = Trim(ln.substr(0, colon));
            std::string value = Trim(ln.substr(colon + 1));

            if (key == "ResourceSold") {
                mainResource = StringToResource(value);
                hasResource = true;
            }
            else if (key == "Gold") {
                // Parse integer -> inventory.money
                try {
                    int gold = std::stoi(value);
                    shop.inventory.money = gold;
                }
                catch (...) {
                    shop.inventory.money = 0;
                    std::cerr << "ShopTemplateLoader: could not parse Gold value: '" << value << "' for shop: " << shop.name << "\n";
                }
            }
            else if (key == "Stock") {
                // Parse stock integer -> add to shop.inventory of the main resource
                int stock = 0;
                try {
                    stock = std::stoi(value);
                }
                catch (...) {
                    stock = 0;
                }
                if (hasResource) {
                    shop.inventory.Add(mainResource, stock);
                }
                else {
                    // In case ResourceSold appears after Stock (unlikely per agreed format),
                    // attempt to add later — here we just warn and skip adding.
                    std::cerr << "ShopTemplateLoader: 'Stock' found before 'ResourceSold' in template '" << shop.name << "'. Stock ignored.\n";
                }
            }
            else if (key == "Priorities") {
                // Read up to ResourceType::Count integers from value
                std::istringstream pis(value);
                for (int i = 0; i < static_cast<int>(ResourceType::Count); ++i) {
                    int p = 0;
                    if (!(pis >> p)) p = 0;
                    shop.priorities[i] = p;
                }
            }
            else {
                // Unknown key — ignore but warn
                std::cerr << "ShopTemplateLoader: unknown key '" << key << "' in template for shop: " << shop.name << "\n";
            }
        }

        parsed.push_back(shop);
    }

    // 3) Shuffle parsed templates and return up to 'count' (no duplicates)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(parsed.begin(), parsed.end(), gen);

    int take = std::min(count, static_cast<int>(parsed.size()));
    std::vector<Shop> result(parsed.begin(), parsed.begin() + take);
    return result;
}