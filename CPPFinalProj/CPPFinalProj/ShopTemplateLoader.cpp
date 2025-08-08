#include "ShopTemplateLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include "ResourceUtils.h"
#include "StringUtils.h" // We'll define this below
#include "Inventory.h"
#include "config.h"


std::vector<Shop> ShopTemplateLoader::LoadRandomTemplates(int count, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open template file: " << filePath << "\n"; //exits with error if no file is found
        exit(1);
    }

	std::vector<std::string> rawTemplates; // Stores all templates as raw strings
    std::string line, currentBlock;

    while (std::getline(file, line)) {
        if (line == "---") {
            if (!currentBlock.empty()) {
                rawTemplates.push_back(currentBlock); // Saves a template as a string in rawTemplates
                currentBlock.clear();
            }
        }
        else {
            currentBlock += line + "\n";
        }
    }
    if (!currentBlock.empty()) rawTemplates.push_back(currentBlock);

    std::random_device rd;  //Randomization tools
    std::mt19937 gen(rd());
    std::shuffle(rawTemplates.begin(), rawTemplates.end(), gen);

	std::vector<Shop> result; //Vector to store parsed Shop objects

    for (int i = 0; i < count && i < rawTemplates.size(); ++i) {
        std::istringstream iss(rawTemplates[i]);
        std::string line;
        Shop shop;
        shop.name = "Unnamed";
        shop.zone = 0;
        shop.id = i;
        shop.position = { 0, 0 };

        for (int& p : shop.priorities)
            p = 0; // Default priority = 0

        while (std::getline(iss, line)) {
            if (line.find("ShopName:") != std::string::npos) {
                shop.name = line.substr(line.find(":") + 2);
            }
            else if (line.find("Stock:") != std::string::npos) {
                std::getline(iss, line);
                std::string res = line.substr(4, line.find(":") - 4);
                int amount = std::stoi(line.substr(line.find(":") + 2));
                shop.inventory.Add(StringToResource(res), amount);
            }
            else if (line.find("Priorities:") != std::string::npos) {
                while (std::getline(iss, line) && !line.empty()) {
                    std::string res = line.substr(4, line.find(":") - 4);
                    int prio = std::stoi(line.substr(line.find(":") + 2));
                    ResourceType rt = StringToResource(res);
                    shop.priorities[(int)rt] = prio;
                }
            }
        }

		result.push_back(shop); // Add the parsed shop to the result vector
    }

    return result;
}